#include "HitObject.h"

HitObject::HitObject() {
  isHit = false;
  point = Vector3f(0,0,0);
  disVec = Vector3f(0, 0, 0);
  normal = Vector3f(0,1,0);
  finish.amb = finish.diff = finish.spec = finish.refl = 0;
  near = 0;
  color.r = color.g = color.b = 0;
  theta = 0;
}
