#include "main.h"

int imageWidth;               //Width of the created Image      
int imageHeight;              //Height of the created Image
string inputFile;             //Name of the povray file to be parsed
Color backgroundColor;        //Default background Color
bool isDebugMode = false;     //Boolean for debugMode on/off
int debugLevel = 0;           //Level of detail for debugging (1 or 2)
bool debugThisPoint = false;  //Boolean for debugging current point
int antiAliasingMode = 1;     //AA on = 1, off = 0, default 1
bool isGlobalIllum = false;   //Boolean for GI on/off

/*
 * The root of all problems function
 */
int main (int argc, char *argv[]) {
  Parser p;
  vector<DrawableObject *> olist;
  vector<Plane *> planes;
  vector<LightSource *> lights;
  Camera cam;
  Scene *scene;

  //Manage commandline arguments
  checkArgs(argc, argv);

  //create scene
  scene = new Scene(&cam, &olist, &planes, &lights, isDebugMode); 

  //Parse file / populate scene
  p.parseFile(inputFile, scene);

  //Create bvhNode tree
  scene->root = new bvhNode(scene->objects, 1, scene->objects->size());

  if (isDebugMode && debugLevel == 2)
    printTree(scene->root);
  
  manageRaycast(scene);

  //if (isDebugMode)
    //  printValues(scene);
}

/*
 * ------------------------------------------------------------------------
 * ----------------------------- RayCast Functions -------------------------
 * ------------------------------------------------------------------------
 */

/*
 * Function that manages the casting flow for the entire program.
 * Loops through each pixel, creating the corresponding ray and sends it,
 *  setting the current pixel to the returned color.
 */
void manageRaycast(Scene *scene) {
  Color resClr, tempClr, sumClr;
  View view;
  Ray ray;
  int counter = 1;
  int percent = (imageWidth * imageHeight) / 10;
  int prcCnt = 10;
  
  //Initialize image
  Image img(imageWidth, imageHeight);
  color_t clrc; //471 code used different color structure

  //Get view frustum
  view = getView(scene);

  //Initialize background color
  backgroundColor.r = backgroundColor.g = backgroundColor.b = 0;

  //Loop over all the pixels
  //#pragma omp parallel for
  for (int i = 0; i < imageWidth; i++) {
    for (int j = 0; j < imageHeight; j++) {
      if ( ++counter == percent) {
	printf("At %d%% of primary rays sent\n", prcCnt);
	percent += (imageWidth * imageHeight) / 10;
	prcCnt += 10;
      }

      if (isDebugMode && (i == 417 && j == 303))
	debugThisPoint = true;
      
      //Send primary rays
      resClr = sendPrimaryRays(view, i, j, scene);

      if (debugThisPoint)
	debugThisPoint = false;
      
      //Set pixel color structure from my structure
      clrc.r = resClr.r, clrc.g = resClr.g, clrc.b = resClr.b;
      
      //Color the current pixel with the closest hit color
      img.pixel(i, j, clrc);
    }
  }

  //Write tga file
  img.WriteTga((char *)"result.tga", true);
}

/*
 * Function that manages sending the primary rays, using AA if set
 */
Color sendPrimaryRays(View view, int i, int j, Scene *scene) {
  Color resClr, sumClr ={0,0,0,1}, tempClr;
  Ray ray;

  if (antiAliasingMode) { //true == AA, false = no AA
    for (int k = 0; k < AA_NUM; k++) {
      //Get dist ray
      ray = getDistRay(view, i, j, k, scene);
      ray.ior = IOR_AIR; //set index to air
      
      //Raycast for the color of this pixel
      tempClr = raytrace(ray, scene, RECURSE_DEPTH);
      sumClr.r += tempClr.r;
      sumClr.g += tempClr.g;
      sumClr.b += tempClr.b;
    }

    //Average color by number of AA rays sent
    resClr.r = sumClr.r / (float)AA_NUM;
    resClr.g = sumClr.g / (float)AA_NUM;
    resClr.b = sumClr.b / (float)AA_NUM;
    
  } else { //No AA
    ray = getDistRay(view, i, j, -1, scene);
    ray.ior = IOR_AIR;
    if (isGlobalIllum)
      resClr = raytrace(ray, scene, 2); //only 2 levels of recursion
    else
      resClr = raytrace(ray, scene, RECURSE_DEPTH);
  }
    
  return resClr;
}

/*
 * Recursive raytrace function.
 * Casts the given ray, and calls reflection, refractions or color
 *  computation functions as necessary
 */
Color raytrace(Ray ray, Scene *scene, int depthCount) {
  HitObject hit;
  Color finalColor, refrColor = {0,0,0,1};
  Color reflColor = {0,0,0,1}, localColor;
  float R;
  
  //Get the hitobject for this ray
  hit = castRay(&ray, scene);

  //Check for a valid hit
  if (hit.isHit) {

    if (debugThisPoint) {
      printf("Ray: (%f, %f, %f) -> (%f, %f, %f)\n",
	     ray.point.x(), ray.point.y(), ray.point.z(),
	     ray.dir.x(), ray.dir.y(), ray.dir.z());
      printf("T: %f, Intersection: (%f, %f, %f)\n",
	     hit.near, hit.point.x(), hit.point.y(), hit.point.z());
    }
    
    if (depthCount > 0) {
      //Check if refraction computation is required
      if (hit.finish.refr == 1) {
	refrColor = refract(hit, ray, scene, depthCount - 1);
      }

      //Check if reflection computation is required
      if (hit.finish.refl > 0) {
	reflColor = reflect(hit, ray, scene, depthCount - 1);
      }
    }
    
    //Get the BRDF color of this point
    localColor = computeColor(hit, scene, depthCount);
    
    //Compute final color for combination of refl/refr/local colors
    if (hit.finish.refr == 0) {
      if (hit.finish.refl == 0) {
	finalColor = localColor; //Always here for GI
	
      } else {
	finalColor = computeReflColor(hit.finish.refl, localColor, reflColor);
	
      }
    } else {
      R = computeRValue(hit.finish.ior, hit.theta);
      finalColor = computeRefrColor(R, reflColor, refrColor);
    }
    
  } else {
    //If no hit, return the background color
    finalColor = backgroundColor;
  }
  
  return finalColor;
}

/*
 * Function that controls the refraction computation, generating the 
 *  refracted ray and calling raytrace with it
 */
Color refract(HitObject hit, Ray ray, Scene *scene, int depthCount) {
  Ray newRay;
  Vector3f t;
  Color res;
  
  if ((-ray.dir).dot(hit.normal) < 0) {
    //Angle > 90, inside an object, give negative normal
    t = computeRefracVec(hit.finish.ior, IOR_AIR, ray.dir, -hit.normal);    
    
  } else {
    //otherwise normal hit, give regular normal
    t = computeRefracVec(IOR_AIR, hit.finish.ior, ray.dir, hit.normal);
  
  }

  if (t.x() == 0 && t.y() == 0 && t.z() == 0) {
    return backgroundColor;
  }

  //Construct new ray
  newRay.point = hit.point + (hit.disVec * DISPLACE_VALUE);
  newRay.dir = t;
  newRay.dir.normalize();
  
  return raytrace(newRay, scene, depthCount);
}

/*
 * Function that computes the 't' vector involved in refraction
 */
Vector3f computeRefracVec(float n1, float n2, Vector3f d, Vector3f N) {
  Vector3f result;
  float ratio, dot, rad, temp;

  //Compute radicand
  ratio = n1/n2;
  dot = N.dot(-d);
  rad = 1 - ( pow(ratio, 2) * (1 - pow(dot, 2)));

  //Check for imaginary number, returning 0 if found
  if (rad < 0) {
    result = Vector3f(0,0,0);

  } else {
    temp = sqrt(rad);
    result = ratio * (d + (dot * N)) - (N * temp); 
  }
  
  return result;
}

/*
 * Function that creates the reflected ray and calls raycast on hit,
 *  returning the resulting color
 */
Color reflect(HitObject hit, Ray ray, Scene *scene, int depthCount) {
  Vector3f R;
  Ray newRay;
  Color res;
  
  //Compte reflection vector direction
  R = ray.dir + 2 * (hit.normal.dot(-ray.dir)) * hit.normal;
  
  //Set up ray, moving point in order to avoid a self-hit
  newRay.point = hit.point + (-hit.disVec * DISPLACE_VALUE);
  newRay.dir = R;
  newRay.ior = ray.ior;
  newRay.dir.normalize();   

  res = raytrace(newRay, scene, depthCount); //recursive call

  return res;
}

/*
 * Function that casts the given ray, checking against all objects
 */
HitObject castRay(Ray *ray, Scene *scene) {
  double closest = INFINITY;
  HitObject treeRes, planeRes, temp, defaultRes;
  bool hitTree = false, hitPlane = false;
  
  //Check with non-plane geometry
  hitTree = scene->root->hit(ray, &treeRes);
  
  //Check with planes
  for (int i = 0; i < scene->planes->size(); i++) {
    temp = scene->planes->at(i)->getIntersections(ray);

    if (temp.isHit && temp.near < closest) {
      closest = temp.near;
      planeRes = temp;
      hitPlane = true;
    }
  }

  //If both geometry and plane hit, get closest
  if (hitTree && hitPlane) {
    if (planeRes.near < treeRes.near) {
      return planeRes; //plane is closer

    } else {
      return treeRes; //other shape is closer
    }

    //Otherwise get the object that was hit
  } else if (hitTree) {
      return treeRes;

  } else if (hitPlane) {
    return planeRes;

  } else
    return defaultRes;
}

/*
 * ------------------------------------------------------------------------
 * ----------------------------- Color Functions -------------------------
 * ------------------------------------------------------------------------
 */

Color computeReflColor(float refl, Color locColor, Color reflColor) {
  Color res;

  res.r = (1 - refl) * locColor.r + refl * reflColor.r;
  res.g = (1 - refl) * locColor.g + refl * reflColor.g;
  res.b = (1 - refl) * locColor.b + refl * reflColor.b;
  
  return res;
}

Color computeRefrColor(float R, Color reflColor, Color refrColor) {
  Color res;

  res.r = (R * reflColor.r) + (1 - R) * refrColor.r;
  res.g = (R * reflColor.g) + (1 - R) * refrColor.g;
  res.b = (R * reflColor.b) + (1 - R) * refrColor.b;  
  
  return res;
}

/*
 * Function that computes the R value for color computation
 */
float computeRValue(float ior, float cosTheta) {
  float res, n1, n2, r0, temp, sum;
  Vector3f H, V, L;
  
  n1 = ior;
  n2 = IOR_AIR;

  r0 = (n2 - n1) / (n2 + n1);

  r0 = pow(r0, 2);
  
  res = r0 + (1 - r0) * pow(1 - cos(cosTheta), 5);  
    
  return res;
}

/*
 * Compute the Blinn-Phong color of the point
 */
Color computeColor(HitObject hit, Scene *scene, int depthCount) {
  Color amb, diff, result;

  //Check for global illuminatino
  if (isGlobalIllum) {
    //If count is not yet done, send GI rays, otherwise just get BRDF color
    if (depthCount > 0) {
      amb = getAmbientColor(hit, scene, depthCount);
      diff = getBRDFColor(hit, scene);

      result.r = amb.r + diff.r;
      result.g = amb.g + diff.g;
      result.b = amb.b + diff.b;
      
    } else {
      result = getBRDFColor(hit, scene);
      
    }
  } else {
    //No GI, get amb + BRDF as normal
    amb = getAmbientColor(hit, scene, depthCount);
    diff = getBRDFColor(hit, scene);
      
    result.r = amb.r + diff.r;
    result.g = amb.g + diff.g;
    result.b = amb.b + diff.b;
  }

  //Clamp values to 0
  if (result.r < 0)
    result.r = 0;
  if (result.g < 0)
    result.g = 0;
  if (result.b < 0)
    result.b = 0;
  
  return result;
}

int blargo = 0;

Color getGlobalIllum(HitObject hit, Scene *scene, int depthCount) {
  Color res, sum = {0,0,0,1}, temp;
  float angle;
  Vector3f zvec = Vector3f(0,0,1), N = hit.normal;
  Vector3f axis, randDir;
  Ray Li;
  Matrix3f rot;
  int numRays = depthCount * 64; //128 --> 64
  bool rotRay = true;

  /*
  if (depthCount == 2) {
    if (blargo++ < 50)
    printf("GI normal: (%f, %f, %f)\n", hit.normal.x(),
	   hit.normal.y(), hit.normal.z());
    else
      exit(0);
      }*/
  
  //All random rays start at hit point + offset
  Li.point = hit.point + (-hit.disVec * DISPLACE_VALUE);

  //Compute angle and axis
  angle = N.dot(zvec); //dot
  if (angle < 0) {
    angle = N.dot(-zvec);
  }

  angle = acos(angle); //angle
  axis = zvec.cross(N);

  //Axis parallel to N, dont need to rotate
  if (axis.norm() < .001) {
    //Normal parallel to Z, no need to rotate
    rotRay = false;
    
  } else { //Otherwise rotate the ray to be in direction of normal    
    //Eigen wants a normalized axis
    AngleAxis<float> aa(angle, axis.normalized()); //rot
    rot = aa;
  }

  //Loop over the number of rays 
  for (int i = 0; i < numRays; i++) {
    //Get random direction for the ray
    randDir = getRandomRayDir();
    
    //Get ray direction from rotated randomray
    if (rotRay)
      Li.dir = rot * randDir;
    else
      Li.dir = randDir;
    
    Li.dir.normalize();

    //if (i < 10)
    //printf("ray: (%f, %f, %f)\n", Li.dir.x(), Li.dir.y(), Li.dir.z());

    if (Li.dir.dot(N) < 0) { //ray not in dir of normal for some reason
      Li.dir = -Li.dir;
    }

    assert((Li.dir.dot(N) >= 0));
      
    //cast ray
    temp = raytrace(Li, scene, depthCount - 1);
    
    //sum
    sum.r += temp.r;
    sum.g += temp.g;
    sum.b += temp.b;
  }
  
  //compute final color
  res.r = (hit.color.r / numRays) * sum.r;
  res.g = (hit.color.g / numRays) * sum.g;
  res.b = (hit.color.b / numRays) * sum.b;
  
  return res;
}

Vector3f getRandomRayDir() {
  Vector3f res;
  
  //Get random values [0, 1]
  float u1 = (float)rand() / (float)RAND_MAX;
  float u2 = (float)rand() / (float)RAND_MAX;

  //Get radius and theta
  float r = sqrt(u1);
  float theta = 2.0 * M_PI * u2;

  //Get xyz values and make vector
  float x = r * cos(theta);
  float y = r * sin(theta);
  float z = sqrt(fmax(0, 1.0 - (x * x) - (y * y)));

  res = Vector3f(x,y,z);

  return res;
}

/*
 * Helper function for computing color, gets the ambient component
 */
Color getAmbientColor(HitObject hit, Scene *scene, int depthCount) {
  Color res = {0,0,0,1};

  if (isGlobalIllum) {
    return getGlobalIllum(hit, scene, depthCount);
    
  } else {  
    res.r = hit.color.r * hit.finish.amb;
    res.g = hit.color.g * hit.finish.amb;
    res.b = hit.color.b * hit.finish.amb;

    return res;
  }
}

/*
 * Function that gets the BRDF color of the shape at the hit point
 */
Color getBRDFColor(HitObject hit, Scene *scene) {
  Vector3f N, L, H, V;
  float diffDot, specDot;
  Finish finish = hit.finish;
  Ray lray;
  Color temp = {0,0,0,1}, lclr;
  
  //View, Normal, and lray point don't change with different lights
  V = scene->camera->location - hit.point;
  V.normalize();
  N = hit.normal;
  lray.point = hit.point + (-hit.disVec * DISPLACE_VALUE);
  
  //Loop over all lights
  for (int i = 0; i < scene->lights->size(); i++) {
    //Create ray vector
    L = (scene->lights->at(i))->location - lray.point;
    L.normalize();
    lray.dir = L;

    //Send shadow ray to current light
    if (!(castRay(&lray, scene)).isHit) { //If there is not hit, add to color
      //Get half vector
      H = (V + L) / 2;
      H.normalize();
      
      //get current light's color
      lclr = scene->lights->at(i)->color;
      
      //Get dot products
      diffDot = N.dot(L);
      if (isGlobalIllum)
	specDot = 0;
      else {
	if (finish.rough == 0) {
	  specDot = 1;
	} else {
	  specDot = pow( H.dot(N), 1.0 / finish.rough);
	}
      }
      
      //Compute running sum of all the colors
      //Compute red
      temp.r += (lclr.r * diffDot * (hit.color.r * finish.diff)) + //Diffuse
	(lclr.r * specDot * (hit.color.r * finish.spec)); //Specular
      //Compute green
      temp.g += (lclr.g * diffDot * (hit.color.g * finish.diff)) +
	(lclr.g * specDot * (hit.color.g * finish.spec));
      //Compute blue
      temp.b += (lclr.b * diffDot * (hit.color.b * finish.diff)) +
	(lclr.b * specDot * (hit.color.b * finish.spec));
    }
    //If there is a shadow hit, do not add to the color
  }

  return temp;
}

/*
 * ------------------------------------------------------------------------
 * ----------------------------- View Functions ---------------------------
 * ------------------------------------------------------------------------
 */

/*
 * Creates the 'd' ray based on the camera's view and the current pixel
 */
Ray getDistRay(View v, int i, int j, int k, Scene *scene) {
  float us, vs, ws, offset;
  Vector3f U, V, W, sprime;
  Ray res;

  //get offset, with or without AA
  if (k == -1) //no AA
    offset = .5;
  else
    offset = getDistOffset(k);

  //Get world coordinates
  us = v.left + (v.right - v.left) * ( (i + offset) / imageWidth);
  vs = v.bottom + (v.top - v.bottom) * ( (j + offset) / imageHeight);
  ws = -1;

  //Get World frame
  U = (scene->camera->rightVec) / (scene->camera->rightVec).norm();
  V = scene->camera->upVector;
  W = -(scene->camera->lookAt - scene->camera->location) /
    (scene->camera->lookAt - scene->camera->location).norm();

  sprime = scene->camera->location +
    (us * U) + (vs * V) + (ws * W); //s' = P0+Us(U)+Vs(V)+Ws(W)
  res.point = scene->camera->location;
  res.dir = sprime - scene->camera->location; //d = s' - P0
  res.dir.normalize();
  
  return res;
}

/*
 * Function that gets a jittered offset based on the current AA ray
 */
float getDistOffset(int k) {
  float temp;
  
  if (k < 3) {
    temp = (rand() % 33) / 33;
    
  } else if (k < 6) {
    temp = ((rand() % 33) / 33) + .33;

  } else {
    temp = ((rand() % 33) / 33) + .66;
    
  }

  if (temp > 1 || temp < 0)
    printf("invalid offset: %f\n", temp);
  
  return temp;
}

View getView(Scene *scene) {
  View v;

  v.right = scene->camera->rightVec.norm() / 2;
  v.left = - v.right;
  v.top = scene->camera->upVector.norm() / 2;
  v.bottom = - v.top;

  return v;
}

/*
 * ------------------------------------------------------------------------
 * -------------------------- Commandline Functions -----------------------
 * ------------------------------------------------------------------------
 */

/*
 * Function that parses the commandline arguments, checking for incorrect
 *  input and storing relevant values
 */
void checkArgs(int argc, char *argv[]) {
  int temp;

  if (argc == 6)
    isDebugMode = false;
  else if (argc != 7) {
    cout << "Invalid use." << endl;
    cout << "Usage is: raytrace <width> <height> <input_filename>"
      "<AA mode> <debugMode(optional)>" << endl;
    exit(0);
  }

  temp = atoi(argv[1]);

  if (temp <= 0) {
    cout << "Error: Width must be greater than 0, is " << temp << endl;
    exit(0);
  } else {
    imageWidth = temp;
  }

  temp = atoi(argv[2]);

  if (temp <= 0) {
    cout << "Error: Height must be greater than 0, is " << temp << endl;
    exit(0);
  } else {
    imageHeight = temp;
  }
  
  inputFile = argv[3];

  antiAliasingMode = atoi(argv[4]);

  if (atoi(argv[5]) > 0)
    isGlobalIllum = true;
  else
    isGlobalIllum = false;
  
  if (argc == 7) {
    temp = atoi(argv[6]);
    
    if (temp == 1)
      isDebugMode = true;
    else
      isDebugMode = false;
  }
}

/*
 * ------------------------------------------------------------------------
 * ----------------------------- Debug Functions -------------------------
 * ------------------------------------------------------------------------
 */

/*
 * Function that prints out the boundbox for each element in the tree
 */
void printTree(bvhNode *node) {

  if (node->left != NULL && node->right != NULL) {
    node->bbox.printValues(false);
    printTree(node->left);
    printTree(node->right);

  } else { //is a leaf
    node->bbox.printValues(true);
    node->bbox.myObj->printValues();
  }  
}

/*
 * Debug function that prints out the values of all the scene objects
 */
void printValues(Scene *scene) {
  printf("Camera:\n");
  printf("\tlocation: (%f, %f, %f)\n", scene->camera->location.x(),
	 scene->camera->location.y(),
	 scene->camera->location.z());
  printf("\tupVector: (%f, %f, %f)\n", scene->camera->upVector.x(),
	 scene->camera->upVector.y(), scene->camera->upVector.z());
  printf("\trightVec: (%f, %f, %f)\n", scene->camera->rightVec.x(),
	 scene->camera->rightVec.y(), scene->camera->rightVec.z());
  printf("\tlookAt: (%f, %f, %f)\n", scene->camera->lookAt.x(),
	 scene->camera->lookAt.y(), scene->camera->lookAt.z());

  for (int i = 0; i < scene->lights->size(); i++) {
    printf("Light #%d:\n\tLocation: (%f, %f, %f)\n\tColor: (%f, %f, %f)\n",
	   i,
	   (scene->lights)->at(i)->location.x(),
	   (scene->lights)->at(i)->location.y(),
	   (scene->lights)->at(i)->location.z(),
	   (scene->lights)->at(i)->color.r,
	   (scene->lights)->at(i)->color.g,
	   (scene->lights)->at(i)->color.b);
  }
  
  for (int i = 0; i < scene->objects->size(); i++) {
    (scene->objects)->at(i)->printValues();
  }

}
