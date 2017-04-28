#pragma once

#include "raycaster.h"
#include "HitObject.h"
#include "BoundBox.h"
//#include "DrawableObject.h"

class DrawableObject;

class bvhNode {
 public:
  bvhNode();
  bvhNode(vector <DrawableObject *> *objs, int axis, int n);
  
  bool hit(Ray *ray, HitObject *hit);
  BoundBox combine(BoundBox one, BoundBox two);
  void sortlist(vector <DrawableObject *> *objs, int axis);
  bool compare(DrawableObject *one, DrawableObject *two, int axis);

  bvhNode *left;
  bvhNode *right;
  BoundBox bbox;
};
