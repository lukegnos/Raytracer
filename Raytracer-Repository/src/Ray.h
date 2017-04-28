#pragma once

#include "raycaster.h"

class Ray {
 public:
  Ray();
  Ray(Vector3f p, Vector3f d);
  Ray(Vector3f p, Vector3f d, float n);
  
  Vector3f point;
  Vector3f dir;
  float ior;
};
