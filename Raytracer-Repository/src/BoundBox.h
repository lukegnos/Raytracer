#pragma once

#include "raycaster.h"
//#include "DrawableObject.h"
#include "HitObject.h"
#include "Ray.h"

class DrawableObject;

class BoundBox {
 public:
  BoundBox();
  BoundBox(Vector3f min, Vector3f max, DrawableObject *obj, Matrix4f CTM);
  
  HitObject getIntersections(Ray *ray);
  void printValues(bool isLeaf);
  void getTransformedMinMax();
  float getMinFromArray(float arr[], int size);
  float getMaxFromArray(float arr[], int size);
  Vector3f getTransformedPoint(Vector3f op, Matrix4f m);
  
  Vector3f min;
  Vector3f max;
  DrawableObject *myObj;
  Matrix4f CTM;
};
