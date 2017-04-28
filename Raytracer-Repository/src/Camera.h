#pragma once

#include "raycaster.h"

class Camera {
 public:
  Camera();
  Camera(Vector3f p, Vector3f up, Vector3f r,
	 Vector3f la);
  Vector3f location;
  Vector3f upVector;
  Vector3f rightVec;
  Vector3f lookAt;
};
