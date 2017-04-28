#pragma once

#include "raycaster.h"

class LightSource {
 public:
  LightSource();
  LightSource(Vector3f l, Color c);
  
  Vector3f location;
  Color color;
};
