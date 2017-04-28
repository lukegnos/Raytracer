#pragma once

#include "raycaster.h"
#include "DrawableObject.h"
#include "HitObject.h"
#include "Finish.h"
#include "Scene.h"

class Triangle : public DrawableObject {
 public:
  Triangle();
  Triangle(Vector3f pa, Vector3f pb, Vector3f pc, Color c,
	   Finish f, Vector3f cen, Matrix4f ctm);

  void printValues();
  HitObject getIntersections(Ray *ray);
  Ray getTransformRay(Ray *ray, Matrix4f inverse);
  BoundBox getBoundingBox();
  Vector3f getTransformedPoint(Vector3f op, Matrix4f m);
  
  Vector3f pointA;
  Vector3f pointB;
  Vector3f pointC;
  Color color;
  Finish finish;
  Vector3f tcenter;
  Vector3f normal;
  Matrix4f CTM;
  
 private:
  Vector3f setNormal();
  void getValues(Ray *ray, HitObject *res);
};
