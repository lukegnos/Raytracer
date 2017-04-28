#include "Finish.h"

Finish::Finish() {
  amb = diff = spec = rough = refl = 0.0;
}

Finish::Finish(float a, float d, float s, float r,
	       float ref, float rfr, float ir) {
  amb = a;
  diff = d;
  spec = s;
  rough = r;
  refl = ref;
  refr = rfr;
  ior = ir;
};
