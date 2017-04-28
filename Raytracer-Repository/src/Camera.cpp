#include "Camera.h"

Camera::Camera() {
  location = Vector3f(0,0,0);
  upVector = Vector3f(0,0,0);
  rightVec = Vector3f(0,0,0);
  lookAt = Vector3f(0,0,0);
}

Camera::Camera(Vector3f p, Vector3f up, Vector3f r, Vector3f la) {
  location = p;
  upVector = up;
  rightVec = r;
  lookAt = la;
}
