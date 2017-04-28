#include "Sphere.h"

//Constructors
Sphere::Sphere() {
  tcenter = center = Vector3f(0,0,0);
  color.r = 0, color.g = 0, color.b = color.f = 0;
  radius = 0;
  finish.amb = finish.diff = finish.spec = finish.rough = 0;
  CTM = Matrix4f::Identity();
}

Sphere::Sphere(Vector3f p, Color c, double r, Finish f, Matrix4f ctm) {
  center = p;
  color = c;
  radius = r;
  finish = f;
  CTM = ctm;

  if (!CTM.isIdentity())
    tcenter = getTransformedPoint(center, CTM.inverse());
  else
    tcenter = center;
}

/*
 * Function that returns the transformed point of op, with matrix m
 */
Vector3f Sphere::getTransformedPoint(Vector3f op, Matrix4f m) {
  Vector4f temp;

  temp = Vector4f(op.x(), op.y(), op.z(), 1);
  temp = m * temp;

  return Vector3f(temp.x(), temp.y(), temp.z());
}

/*
 * Function that transforms and returns the transformed ray given a matrix
 */
Ray Sphere::getTransformRay(Ray *ray, Matrix4f inverse) {
  Ray res;
  Vector4f temp;

  //get direction
  temp = Vector4f(ray->dir.x(), ray->dir.y(), ray->dir.z(), 0);
  temp = inverse * temp;
  res.dir = Vector3f(temp.x(), temp.y(), temp.z());

  //get point
  temp = Vector4f(ray->point.x(), ray->point.y(), ray->point.z(), 1);
  temp = inverse * temp;
  res.point = Vector3f(temp.x(), temp.y(), temp.z());
  
  return res;
}

/*
 * Function that gets the hit info for a sphere
 */
HitObject Sphere::getIntersections(Ray *ray) {
  HitObject res;
  double rad, ddot, b;
  Vector3f N, norTemp;
  Vector4f temp;
  Ray tray;
  Matrix4f inverse = CTM.inverse();

  //get transformed ray
  tray = getTransformRay(ray, inverse);
    
  //Get values for radicand computation
  ddot = (tray.dir).dot(tray.dir);
  b = (tray.dir).dot(tray.point - center);
  
  //get the radicand
  rad = getRadicand(ddot, b, tray.dir, tray.point);

  //Check if the values result in a hit
  checkValues(rad, &res, ddot, b);

  if (res.isHit) { //If is a hit, populate result values
    //Get point 
    res.point = ray->point + (ray->dir)*(res.near); //p = p0 + dt
    norTemp = tray.point + (tray.dir)*(res.near);
    N = (norTemp - center) / radius;
    N.normalize(); //need this???

    //apply inverse transpose to normal
    temp = Vector4f(N.x(), N.y(), N.z(), 0);
    temp = inverse.transpose() * temp;
    N = Vector3f(temp.x(), temp.y(), temp.z());
    N.normalize();

    //Populate other hit object values
    res.zoesPoint = tray.point;
    
    res.disVec = tray.dir;
    res.theta = tray.dir.dot(N);    
    res.normal = N;
    res.color = color;
    res.finish = finish;
  }

  return res;
}

/*
 * Function that gets and checks the t-value for the sphere, putting it into
 *  the hitobject
 */
void Sphere::checkValues(double rad, HitObject *res, double ddot, double b) {
  double t1, t2, sqroot;

  //Check radicand
  if (rad < 0) { //No hit
    res->isHit = false;

  } else if (rad == 0) { //1 hit, get tvalue
    res->isHit = true;
    
    //Get the t value
    res->near = -b / ddot;

  } else { //multiple hits on ray somewhere
    res->isHit = true;    
    sqroot = sqrt(rad);

    //get both options
    t1 = (-b + sqroot) / ddot;
    t2 = (-b - sqroot) / ddot;
    
    //If both negative, no hits (behind camera)
    if (t1 < 0 && t2 < 0) {
      res->isHit = false;
      
    } else { //At least one valid hit
      
      if (t1 < 0) { //t1 behind camera, return t2
	res->near = t2;
	
      } else if (t2 < 0) { //t2 behind camera, return t1
	res->near = t1;

      } else { //could be either, find nearest
	if (t1 <= t2) {
	  res->near = t1;

	} else {
	  res->near = t2;
	  
	}
      }
    }
  }
}

/*
 * Helper function that gets the value under the radicand for sphere inters.
 */
double Sphere::getRadicand(double ddot, double b,
			   Vector3f dist, Vector3f p0) {
  Vector3f temp;
  double dotres;

  temp = p0 - center;
  dotres = ddot * (temp.dot(temp) - pow(radius, 2));

  return pow(b, 2) - dotres;
}

/*
 * Get the bounding box for this shape
 */
BoundBox Sphere::getBoundingBox() {
  Vector3f min, max;
  float offset = .01;
    
  //Get non-transformed min/max
  min = Vector3f(center.x() - radius - offset, center.y() - radius - offset,
		 center.z() - radius - offset);

  max = Vector3f(center.x() + radius + offset, center.y() + radius + offset,
		 center.z() + radius + offset);

    return BoundBox(min, max, (DrawableObject *)this, CTM);  
}



/*
 * Print out values for object
 */
void Sphere::printValues() {
  cout << "Sphere Values:\n";
  cout << "\tpoint: (" << center.x() << ", " << center.y() << ", "
       << center.z() << ")\n";
  printf("\tcolor:(%f, %f, %f, %f)\n", color.r, color.g, color.b, color.f);
  cout << "\tradius: " << radius << endl;
  printf("\tFinish: \n"
	 "\t\tamb: %.5f\n"
	 "\t\tdiff: %.5f\n"
	 "\t\tspec: %.5f\n"
	 "\t\trough: %.5f\n"
	 "\t\trefl: %.5f\n"
	 "\t\trefr: %.5f\n"
	 "\t\tior: %.5f)\n",
	 finish.amb, finish.diff, finish.spec, finish.rough, finish.refl,
	 finish.refr, finish.ior);
}

