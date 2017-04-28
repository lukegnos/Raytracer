#include "Triangle.h"

//Constructors
Triangle::Triangle() {
  pointA = Vector3f(1,0,0);
  pointB = Vector3f(-1,0,0);
  pointC = Vector3f(0,1,0);
  tcenter = Vector3f(0, .5, 0);
  finish.amb = finish.diff = finish.spec = finish.rough = 0;
  color.r = color.g = color.b = color.f = 0;
  normal = setNormal();
  CTM = Matrix4f::Identity();
}

Triangle::Triangle(Vector3f pa, Vector3f pb, Vector3f pc, Color c,
		   Finish f, Vector3f cen, Matrix4f ctm) {  
  pointA = pa;
  pointB = pb;
  pointC = pc;
  color = c;
  finish = f;
  CTM = ctm;
  normal = setNormal();

  if (!CTM.isIdentity())
    tcenter = getTransformedPoint(cen, CTM.inverse());
  else
    tcenter = cen;
}

/*
 * Function that returns the transformed point of op, with matrix m
 */
Vector3f Triangle::getTransformedPoint(Vector3f op, Matrix4f m) {
  Vector4f temp;

  temp = Vector4f(op.x(), op.y(), op.z(), 1);
  temp = m * temp;

  return Vector3f(temp.x(), temp.y(), temp.z());
}

/*
 * Intersection function for triangle
 */
HitObject Triangle::getIntersections(Ray *ray) {
  HitObject res;
  Ray newray, tray;
  Vector3f N;
  Vector4f temp;
  Matrix4f inverse = CTM.inverse();

  //Get transformed ray
  tray = getTransformRay(ray, inverse);

  //Get 't' and isHit values
  getValues(&tray, &res);

  if (res.isHit) { //If hit, get hit values
    res.point = tray.point + (tray.dir)*(res.near); //p = p0 + dt

    //Transform the normal
    temp = Vector4f(normal.x(), normal.y(), normal.z(), 0);
    temp = inverse.transpose() * temp;
    N = Vector3f(temp.x(), temp.y(), temp.z());
    N.normalize();

    //Make sure normal is facing camera
    if (tray.dir.dot(N) > 0) {
      N = -N;
    }

    //Populate other hit objects
    res.disVec = tray.dir;
    res.theta = tray.dir.dot(N);
    res.normal = N;
    res.color = color;
    res.finish = finish;	
  }
  
  return res;
}

/*
 * Function that sets the triangle normal based off its three points
 */
Vector3f Triangle::setNormal() {
  Vector3f N, U, V;
  double x, y, z;

  U = pointB - pointA;
  V = pointC - pointA;

  N = Vector3f( (U.y() * V.z()) - (U.z() * V.y()),
		(U.z() * V.x()) - (U.x() * V.z()),
		(U.x() * V.y()) - (U.y() * V.x()));
  N.normalize();

  return N;
}

/*
 * Function that gets the 't' values for the ray and this triangle via
 *  the determinants
 */
void Triangle::getValues(Ray *ray, HitObject *res) {
  double beta, gamma, denom;
  Matrix3f A, B, C, D;

  //Create matrices
  A << pointA.x() - pointB.x(), pointA.x() - pointC.x(), ray->dir.x(),
    pointA.y() - pointB.y(), pointA.y() - pointC.y(),  ray->dir.y(),
    pointA.z() - pointB.z(), pointA.z() - pointC.z(), ray->dir.z();

  B << pointA.x() - ray->point.x(), pointA.x() - pointC.x(), ray->dir.x(),
    pointA.y() - ray->point.y(), pointA.y() - pointC.y(), ray->dir.y(),
    pointA.z() - ray->point.z(), pointA.z() - pointC.z(), ray->dir.z();

  C << pointA.x() - pointB.x(), pointA.x() - ray->point.x(), ray->dir.x(),
    pointA.y() - pointB.y(), pointA.y() - ray->point.y(),  ray->dir.y(),
    pointA.z() - pointB.z(), pointA.z() - ray->point.z(), ray->dir.z();

  D << pointA.x() - pointB.x(), pointA.x() - pointC.x(),
    pointA.x() - ray->point.x(),
    pointA.y() - pointB.y(), pointA.y() - pointC.y(),
    pointA.y() - ray->point.y(),
    pointA.z() - pointB.z(), pointA.z() - pointC.z(),
    pointA.z() - ray->point.z();

  denom = A.determinant();

  if (denom == 0) {
    res->isHit = false;
    return;
  }
  
  res->near = D.determinant() / denom;
  res->isHit = true;

  //Check that 't' value is a hit
  if (res->near < 0) {// || res->near > 100) {
    res->isHit = false;
    return;
  }

  gamma = C.determinant() / denom;
  if (gamma < 0 || gamma > 1) {
    res->isHit = false;
    return;
  }

  beta = B.determinant() / denom;
  if (beta < 0 || beta > 1 - gamma) {
    res->isHit = false;
    return;
  }  
}

/*
 * Function that creates the bounding box of the triangle
 */
BoundBox Triangle::getBoundingBox() {
  Vector3f min, max;
  float offset = .01;
  
  min = Vector3f( std::min(pointA.x(), std::min(pointB.x(),
						pointC.x())) - offset,
		  std::min(pointA.y(), std::min(pointB.y(),
						pointC.y())) - offset,
		  std::min(pointA.z(), std::min(pointB.z(),
						pointC.z())) - offset);

  max = Vector3f( std::max(pointA.x(), std::max(pointB.x(),
						pointC.x())) + offset,
		  std::max(pointA.y(), std::max(pointB.y(),
						pointC.y())) + offset,
		  std::max(pointA.z(), std::max(pointB.z(),
						pointC.z())) + offset);
  
  return BoundBox(min, max, (DrawableObject *)this, CTM);
}

/*
 * Function that gets the transformed ray for the given ray and matrix
 */
Ray Triangle::getTransformRay(Ray *ray, Matrix4f inverse) {
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
 * Prints out values for this object
 */
void Triangle::printValues() {
  cout << "Triangle Values:" << endl;
  printf("\tPointA: (%f, %f, %f)\n", pointA.x(), pointA.y(), pointA.z());
  printf("\tPointB: (%f, %f, %f)\n", pointB.x(), pointB.y(), pointB.z());
  printf("\tPointC: (%f, %f, %f)\n", pointC.x(), pointC.y(), pointC.z());

  printf("\tColor: (%f, %f, %f, %f)\n", color.r, color.g, color.b, color.f);
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
