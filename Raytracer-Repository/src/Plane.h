#pragma once

#include "raycaster.h"
#include "HitObject.h"
#include "Finish.h"
#include "LightSource.h"
#include "Scene.h"
#include "Ray.h"

class Plane {
 public:
  Plane();
  Plane(Vector3f nor, Color c, double d, Finish f, Matrix4f ctm);
  void printValues();
  HitObject getIntersections(Ray *ray);
  void checkValues(Ray *ray, HitObject *res);
  Ray getTransformRay(Ray *ray, Matrix4f inverse);
  
  Vector3f normal;
  Color color;
  double dist;
  Finish finish;
  Matrix4f CTM;
};
