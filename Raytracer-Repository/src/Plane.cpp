#include "Plane.h"

using namespace std;

//Constructors
Plane::Plane() {
  normal = Vector3f(0,0,0);
  color.r = 0, color.g = 0, color.b = color.f = 0;
  dist = 0;
  finish.amb = finish.diff = finish.spec = finish.rough = 0;
  CTM = Matrix4f::Identity();
}

Plane::Plane(Vector3f nor, Color c, double d, Finish f, Matrix4f ctm) {
  normal = nor;
  color = c;
  dist = d;
  finish = f;
  CTM = ctm;
}

/*
 * Intersect function for plane
 */
HitObject Plane::getIntersections(Ray *ray) {
  HitObject res;
  Ray tray;
  Matrix4f inverse = CTM.inverse();
  Vector4f temp;
  Vector3f N;

  //Get transformed ray
  tray = getTransformRay(ray, inverse);

  //Check 't' and isHit values
  checkValues(&tray, &res);
  
  if (res.isHit) { //If it was a hit, populate hit values
    N = normal;
    //apply inverse transpose to normal
    temp = Vector4f(N.x(), N.y(), N.z(), 0);
    temp = inverse.transpose() * temp;
    N = Vector3f(temp.x(), temp.y(), temp.z());
    N.normalize();

    //Make sure normal is facing camera
    if (tray.dir.dot(N) > 0) {
      N = -N;
    }
    
    //Set other hitObject values
    res.normal = N;
    res.theta = tray.dir.dot(N);
    res.point = ray->point + (ray->dir)*(res.near); //p = p0 + dt
    res.disVec = tray.dir;
    res.color = color;
    res.finish = finish;
    res.disVec = ray->dir;
  }
  
  return res; 
}

/*
 * Function that gets the transformed ray given a ray and a matrix
 */
Ray Plane::getTransformRay(Ray *ray, Matrix4f inverse) {
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
 * Populates the near and isHit fields of the hitobject res
 */
void Plane::checkValues(Ray *ray, HitObject *res) {
  Vector3f p1, temp;
  double t, denom;

  //Get another point on plane
  p1 = normal;
  p1 = p1 * dist;
  
  temp = p1 - ray->point;

  denom = (ray->dir).dot(normal);
  if (denom == 0) {
    res->isHit = false;
    
  } else {
    t = (temp.dot(normal) ) / ((ray->dir).dot(normal));

    res->near = t;
  
    if (t >= 0) {
      res->isHit = true;
      
    } else {
      res->isHit = false;
    }
  }
}

/*
 * Function that prints out object values
 */
void Plane::printValues() {
  cout << "Plane Values:" << endl;
  cout << "\tnormal: (" << normal.x() << ", " << normal.y() << ", "
       << normal.z() << ")\n";
  printf("\tcolor:(%f, %f, %f, %f)\n", color.r, color.g, color.b, color.f);
  cout << "\tdist: " << dist << endl;
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
