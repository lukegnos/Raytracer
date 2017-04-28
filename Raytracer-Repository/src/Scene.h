#pragma once

#include "raycaster.h"
#include "Camera.h"
#include "LightSource.h"
#include "bvhNode.h"
#include "BoundBox.h"
#include "Plane.h"

class DrawableObject;
class Plane;

class Scene {
 public:
  Scene();
  Scene(Camera *c, vector<DrawableObject *> *objs, vector<Plane *> *planes,
	vector<LightSource*> *ls, bool dm);

  Camera *camera;
  vector<DrawableObject *> *objects;
  vector<Plane *> *planes;
  vector<LightSource *> *lights;
  bool debugMode;
  bvhNode *root;
};
