#include "Ray.h"

Ray::Ray() {
  point = Vector3f(0,0,0);
  dir = Vector3f(0,0,0);
  ior = 1;
}

Ray::Ray(Vector3f p, Vector3f d) {
  point = p;
  dir = d;
  ior = 1;
}

Ray::Ray(Vector3f p, Vector3f d, float n) {
  point = p;
  dir = d;
  ior = n;
}
