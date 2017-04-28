#pragma once

#include "raycaster.h"
#include "DrawableObject.h"
#include "HitObject.h"
#include "Finish.h"
#include "Scene.h"

class Sphere : public DrawableObject {
 public:
  Sphere();
  Sphere(Vector3f p, Color c, double r, Finish f, Matrix4f ctm);
  void printValues();
  HitObject getIntersections(Ray *ray);
  Ray getTransformRay(Ray *ray, Matrix4f inverse);
  BoundBox getBoundingBox();
  Vector3f getTransformedPoint(Vector3f op, Matrix4f m);

  Vector3f center;
  Vector3f tcenter;
  Color color;
  double radius;
  Finish finish;
  Matrix4f CTM;
  
 private:
  double getRadicand(double ddot, double b, Vector3f dist, Vector3f p0);
  void checkValues(double rad, HitObject *res, double ddot, double b);
};
