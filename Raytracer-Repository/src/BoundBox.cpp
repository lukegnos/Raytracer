#include "BoundBox.h"
#include "Box.h"

//Constructors
BoundBox::BoundBox() {
  min = max = Vector3f(0,0,0);
  myObj = NULL;
  CTM = Matrix4f::Identity();
}

BoundBox::BoundBox(Vector3f mi, Vector3f ma, DrawableObject *obj,
		   Matrix4f ctm) {
  min = mi;
  max = ma;
  myObj = obj;
  CTM = ctm;
  
  if (!CTM.isIdentity()) {
    getTransformedMinMax();
  }
}

/*
 * Function that transforms all the points of the current box, and
 *  then recomputes a new min and max from the transformed points
 */
void BoundBox::getTransformedMinMax() {
  Vector3f corner, temp;
  Matrix4f inverse = CTM; //Stay in world space
  int i = 0;
  float xs[8], ys[8], zs[8];
  
  //Must transform all points, then get min/max from those points
  corner = min;
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  corner = max;
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  corner = Vector3f(min.x(), min.y(), max.z());
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  corner = Vector3f(min.x(), max.y(), min.z());
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  corner = Vector3f(min.x(), max.y(), max.z());
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  corner = Vector3f(max.x(), min.y(), min.z());
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  corner = Vector3f(max.x(), min.y(), max.z());
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  corner = Vector3f(max.x(), max.y(), min.z());
  temp = getTransformedPoint(corner, inverse);
  xs[i] = temp.x(), ys[i] = temp.y(), zs[i++] = temp.z();

  //Get smallest x-y-z value from all the points
  min = Vector3f(getMinFromArray(xs, 8), getMinFromArray(ys, 8),
		 getMinFromArray(zs, 8));

  //Get largest x-y-z value from all the points
  max = Vector3f(getMaxFromArray(xs, 8), getMaxFromArray(ys, 8),
		 getMaxFromArray(zs, 8));
}

/*
 * Function that gets the smallest number in 'arr' which is of length 'size'
 */ 
float BoundBox::getMinFromArray(float arr[], int size) {
  float temp, smallest = INFINITY;
  
  for (int i = 0; i < size; i++) {
    temp = arr[i];

    if (temp < smallest)
      smallest = temp;    
  }

  return smallest;
}

/*
 * Function that gets the largest number in 'arr' which is of length 'size'
 */
float BoundBox::getMaxFromArray(float arr[], int size) {
  float temp, largest = -INFINITY;
  
  for (int i = 0; i < size; i++) {
    temp = arr[i];

    if (temp > largest)
      largest = temp;    
  }

  return largest;
}

/*
 * Function that returns the transformed point of op, with matrix m
 */
Vector3f BoundBox::getTransformedPoint(Vector3f op, Matrix4f m) {
  Vector4f temp;

  temp = Vector4f(op.x(), op.y(), op.z(), 1);
  temp = m * temp;

  return Vector3f(temp.x(), temp.y(), temp.z());
}

/*
 * Function that gets the intersection of the bounding box by 
 *  creating a Box and using its intersection function
 */
HitObject BoundBox::getIntersections(Ray *ray) {
  HitObject hit;
  Box box;
  
  box = Box(min, max, Matrix4f::Identity());

  return box.getIntersections(ray);
}

/*
 * Prints boundbox values
 */
void BoundBox::printValues(bool isLeaf) {
  printf("BoundBox Values: (%d) \n", isLeaf);
  printf("\tmin: (%f, %f, %f)\n", min.x(), min.y(), min.z());
  printf("\tmax: (%f, %f, %f)\n", max.x(), max.y(), max.z());
}

