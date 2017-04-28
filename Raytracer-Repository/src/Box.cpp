#include "Box.h"

//Constructors
Box::Box() {
  tcenter = corner1 = corner2 = Vector3f(0,0,0);
  finish.amb = finish.diff = finish.spec = finish.rough = 0;
}

Box::Box(Vector3f c1, Vector3f c2, Finish f,
	 Color c, Matrix4f ctm) {
  corner1 = c1;
  corner2 = c2;
  tcenter = Vector3f((corner1.x() + corner2.x()) / 2,
		      (corner1.y() + corner2.y()) / 2,
		      (corner1.z() + corner2.z()) / 2);
  finish = f;
  color = c;
  CTM = ctm;

  if (!CTM.isIdentity())
    tcenter = getTransformedPoint(tcenter, CTM.inverse());
}

//Constructor used by the BoundBox class, no color or finish used
Box::Box(Vector3f v1, Vector3f v2, Matrix4f ctm) {
  corner1 = v1;
  corner2 = v2;
  tcenter = Vector3f((corner1.x() + corner2.x()) / 2,
		      (corner1.y() + corner2.y()) / 2,
		      (corner1.z() + corner2.z()) / 2);
  
  CTM = ctm;

  if (!CTM.isIdentity())
    tcenter = getTransformedPoint(tcenter, CTM.inverse());
}

/*
 * Function that returns the transformed point of op, with matrix m
 */
Vector3f Box::getTransformedPoint(Vector3f op, Matrix4f m) {
  Vector4f temp;

  temp = Vector4f(op.x(), op.y(), op.z(), 1);
  temp = m * temp;

  return Vector3f(temp.x(), temp.y(), temp.z());
}

/*
 * Get intersection function for box
 */
HitObject Box::getIntersections(Ray *ray) {
  HitObject res;
  Matrix4f inverse = CTM.inverse();
  Ray tray;
  Vector3f mins, maxs;
  float tgmin = -INFINITY, tgmax = INFINITY;
  float t1, t2, old;
  int flag = 0;

  //Initialize result to no hit
  res.isHit = false;
  //get transformed ray
  tray = getTransformRay(ray, inverse);
  //Compute min and max values
  computeMinMax(&mins, &maxs);

  //Check x dimension
  if (!checkDimension(&tgmin, &tgmax, mins.x(), maxs.x(),
		      tray.point.x(), tray.dir.x())) { //if false, no hit
    return res;
  }
  
  //Check y dimension
  if (!checkDimension(&tgmin, &tgmax, mins.y(), maxs.y(),
		      tray.point.y(), tray.dir.y())) { //if false, no hit
    return res;
  }

  //Check z dimension
  if (!checkDimension(&tgmin, &tgmax, mins.z(), maxs.z(),
		      tray.point.z(), tray.dir.z())) { //if false, no hit
    return res;
  }

  //Check for hit
  if (tgmin > tgmax || tgmax < 0) //no hit case
    return res;
  else {
    
    if (tgmin < 0) {
      tgmin = tgmax;
      getHitValues(&res, tgmin, &tray, ray, &mins, &maxs, inverse, true);
    } else
      getHitValues(&res, tgmin, &tray, ray, &mins, &maxs, inverse, false);
    
    return res;
  }
}

/*
 * Function that checks if the ray hits a given dimension using the given 
 *  values and updates the tgmin/tgmax values.
 */
bool Box::checkDimension(float *tgmin, float *tgmax, float min,
		    float max, float point, float dir) {
  float t1, t2, temp;

  if (dir != 0) {
    t1 = (min - point) / dir;
    t2 = (max - point) / dir;
    
    if (t2 < t1) { //swap t1, t2 to keep closest on t1
      temp = t1;
      t1 = t2;
      t2 = temp;
    }
    
    if (t1 > *tgmin)
      *tgmin = t1;
    
    if (t2 < *tgmax)
      *tgmax = t2;
    
  } else { //Is parallel ray
    if (point < min || point > max) //parallel doesn't hit
      return false;
  }

  return true;
}

/*
 * Function that gets the min and max values for each dimension and places
 *  the result in the parameter pointers
 */
void Box::computeMinMax(Vector3f *mins, Vector3f *maxs) {
  if (corner1.x() < corner2.x()) {
    (*mins)[0] = corner1.x();
    (*maxs)[0] = corner2.x();
  } else {
    (*mins)[0] = corner2.x();
    (*mins)[0] = corner1.x();
  }

  if (corner1.y() < corner2.y()) {
    (*mins)[1] = corner1.y();
    (*maxs)[1] = corner2.y();
  } else {
    (*mins)[1] = corner2.y();
    (*mins)[1] = corner1.y();
  }

  if (corner1.z() < corner2.z()) {
    (*mins)[2] = corner1.z();
    (*maxs)[2] = corner2.z();
  } else {
    (*mins)[2] = corner2.z();
    (*mins)[2] = corner1.z();
  }
}

/*
 * Function that populates the hitObject with its values
 */
void Box::getHitValues(HitObject *res, float near, Ray *tray,
		       Ray *oray, Vector3f *mins, Vector3f *maxs,
		       Matrix4f inverse, bool flipN) {
  Vector3f nortemp, N;
  Vector4f temp;
  
  res->isHit = true;
  res->color = color;
  res->finish = finish;
  
  res->near = near;
  nortemp = tray->point + tray->dir * near;
  res->point = oray->point + oray->dir * near; //WHAT?!?!?!

  N = getNormal(nortemp, mins, maxs);
  N.normalize();
  
  //apply inverse transpose to normal
  temp = Vector4f(N.x(), N.y(), N.z(), 0);
  temp = inverse.transpose() * temp;
  N = Vector3f(temp.x(), temp.y(), temp.z());
  N.normalize();

  res->disVec = tray->dir;
  if (flipN)
    res->normal = -N;
  else
    res->normal = N;
  
  res->theta = tray->dir.dot(res->normal);
}

/*
 * Function that gets the normal of the box given the hit point and the
 *  mins and maxs for each dimension
 */
Vector3f Box::getNormal(Vector3f p, Vector3f *mins, Vector3f *maxs) {
  Vector3f res = Vector3f(0,0,0);
  float epsi = .01;

  //Check which face the hit point is on, setting the normal as necessary
  if (p.x() <= (*mins).x() + epsi)
    res[0] = -1;
  else if (p.x() >= (*maxs).x() - epsi)
    res[0] = 1;

  else if (p.y() <= (*mins).y() + epsi)
    res[1] = -1;
  else if (p.y() >= (*maxs).y() - epsi)
    res[1] = 1;

  if (p.z() <= (*mins).z() + epsi)
    res[2] = -1;
  else if (p.z() >= (*maxs).z() - epsi)
    res[2] = 1;  
  
  return res;
}

/*
 * Function that gets the transformed ray
 */
Ray Box::getTransformRay(Ray *ray, Matrix4f inverse) {
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
 * Gets the boundingbox for this box, with a small offset
 */
BoundBox Box::getBoundingBox() {
  float offset = .01;
  Vector3f min, max;
  
  min = Vector3f( std::min(corner1.x(), corner2.x()) - offset,
		  std::min(corner1.y(), corner2.y()) - offset,
		  std::min(corner1.z(), corner2.z()) - offset);

  max = Vector3f( std::max(corner1.x(), corner2.x()) + offset,
		  std::max(corner1.y(), corner2.y()) + offset,
		  std::max(corner1.z(), corner2.z()) + offset);
  
  return BoundBox(min, max, (DrawableObject *)this, CTM);
}


/*
 * Prints out box's povray values
 */
void Box::printValues() {
  cout << "Box Values:\n";
  cout << "\tcorner1: (" << corner1.x() << ", " << corner1.y() << ", "
       << corner1.z() << ")\n";
  cout << "\tcorner1: (" << corner2.x() << ", " << corner2.y() << ", "
       << corner2.z() << ")\n";
  printf("\tcolor:(%f, %f, %f, %f)\n", color.r, color.g, color.b, color.f);
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

