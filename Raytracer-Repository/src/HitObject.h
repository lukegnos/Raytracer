#pragma once

#include "raycaster.h"
#include "Finish.h"

class HitObject {
 public:
  HitObject();

  Vector3f zoesPoint;
  
  double near;
  Vector3f point;
  Vector3f disVec;
  bool isHit;
  Vector3f normal;
  Finish finish;
  Color color;
  float theta; //N dot ray
};
