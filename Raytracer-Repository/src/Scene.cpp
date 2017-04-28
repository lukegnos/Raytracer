#include "Scene.h"

Scene::Scene() {

}

Scene::Scene(Camera *c, vector<DrawableObject *> *objs, vector<Plane *> *plns,
	     vector<LightSource*> *ls, bool dm) {
  camera = c;
  lights = ls;
  objects = objs;
  planes = plns;
  debugMode = dm;
}
