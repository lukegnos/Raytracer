#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "raycaster.h"
#include "HitObject.h"
#include "Ray.h"
//#include "BoundBox.h"

class BoundBox;

class DrawableObject {
 public:
  DrawableObject();
  virtual void printValues() = 0;   //Debug function
  virtual HitObject getIntersections(Ray *r) = 0;   //ray hit function
  virtual BoundBox getBoundingBox() = 0;   //bvh tree function
  
  Matrix4f CTM;       //Transformation matrix for the shape
  Vector3f tcenter;   //trasnformed center of the shape (used for sorting)
};
