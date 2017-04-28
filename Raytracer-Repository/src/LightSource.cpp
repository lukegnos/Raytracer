#include "LightSource.h"

LightSource::LightSource() {
  location = Vector3f(0,0,0);
  color.r = color.g = color.b = 1;
}

LightSource::LightSource(Vector3f l, Color c) {
  location = l;
  color = c;
}
