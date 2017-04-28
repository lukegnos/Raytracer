#pragma once

#include "raycaster.h"

class Finish {
 public:
  Finish();
  Finish(float a, float d, float s, float r, float ref,
	 float rfr, float ior);

  float amb;
  float diff;
  float spec;
  float rough;
  float refl;
  float refr;
  float ior;
};
