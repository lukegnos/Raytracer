#include "bvhNode.h"
#include "DrawableObject.h"

//Constructors
bvhNode::bvhNode() {
  left = right = NULL;
}

/*
 * Recursive tree creation constructor, cuts list of object in half every
 *  call if there are more than two elements.
 * Constructed so that every node only has either a left AND right or neither
 *  with leafs having left = right = NULL
 */
bvhNode::bvhNode(vector <DrawableObject *> *objs, int axis, int n) {
  
  if (n == 1) { //if 1 thing in list, node is a leaf
    left = NULL;
    right = NULL;

    //Get the bounding box for the shape that has been placed in this leaf
    bbox = (*objs)[0]->getBoundingBox();
       
  } else if (n == 2) { //If two things in list, either side is a leaf
    left = new bvhNode(objs, axis, 1); //make leaf out of objs[0]
    
    //Make a subvector of objs to give to right node
    vector<DrawableObject *> newlist(objs->begin() + 1, objs->end()); 
    right = new bvhNode(&newlist, axis, 1); //make leaf out of objs[1]
    bbox = combine(left->bbox, right->bbox);
    
  } else { //If more than 2 in list, sort along 'axis' then recursive create    
    sortlist(objs, axis);
    //Cut sorted objs in half, giving left one half and right the other
    left = new bvhNode(objs, (axis + 1) % 3, n/2);
    
    //Make subvector of objs for second half
    vector<DrawableObject *> newlist(objs->begin() + n/2, objs->end()); 
    right = new bvhNode(&newlist, (axis + 1) % 3, n -  n/2);
    bbox = combine(left->bbox, right->bbox);
  }  
}

/*
 * Function that traverses the bvh tree checking for the closest intersection
 *  on the tree's objects if one exists
 * Returns true if the ray hits an object, false if it hits no boundBoxes
 * Populates the HitObject if an object was tested for a hit
 */
bool bvhNode::hit(Ray *ray, HitObject *rec) {
  HitObject temp, Lrec, Rrec;
  bool hitL, hitR;
  hitL = hitR = false;
  
  temp = bbox.getIntersections(ray);
  
  //Query hit on the bbox
  if (temp.isHit) {
    
    //Check if this is a leaf
    if (bbox.myObj != NULL) { //Get shape intersection result if it is
      temp = (bbox.myObj)->getIntersections(ray);
      *rec = temp;
      if (!rec->isHit) { //if it hit box but not shape, return no hit
	return false;
      }
      return true;
      
    } else { //If not a leaf, check both sides of tree
      //Get info on left side of tree if it exists
      if (left != NULL) {
	hitL = left->hit(ray, &Lrec);
      }

      if (right != NULL) {
	hitR = right->hit(ray, &Rrec);
      }

      //If hit both sides of tree, check for closer
      if (hitL && hitR) {

	if (Lrec.near < Rrec.near) { 
	  *rec = Lrec;
	  return true;
	  
	} else {
	  *rec = Rrec;
	  return true;
	}

	//If only hit one side of tree, get which one was hit
      } else if (hitL) {
	*rec = Lrec;
	return true;
	
      } else if (hitR) {
	*rec = Rrec;
	return true;
	
      } else //Return false on no hits
	return false;
    }
  } else //Return false if ray did not hit bounding box
    return false;
}

/*
 * Function that sorts the given vector along the given axis (1=x, 2=y, 3=z)
 */
void bvhNode::sortlist(vector <DrawableObject *> *objs, int axis) {
  DrawableObject *temp;
  int j;
  
  for (int i = 1; i < objs->size(); i++) {
    j = i;

    while (j > 0 && compare((*objs)[j - 1], (*objs)[j], axis)) {
      temp = (*objs)[j];
      (*objs)[j] = (*objs)[j - 1];
      (*objs)[j - 1] = temp;
      j--;
    }
  }
}

/*
 * Returns true if one is greater than two along the given axis, false otherwise
 */
bool bvhNode::compare(DrawableObject *one, DrawableObject *two, int axis) {
  float val1, val2;

  if (axis == 1) {
    val1 = (*one).tcenter.x();
    val2 = (*two).tcenter.x();

    if (val1 > val2)
      return true;
    else
      return false;
    
  } else if (axis == 2) {
    val1 = (*one).tcenter.y();
    val2 = (*two).tcenter.y();

    if (val1 > val2)
      return true;
    else
      return false;
    
  } else if (axis == 3) {
    val1 = (*one).tcenter.z();
    val2 = (*two).tcenter.z();

    if (val1 > val2)
      return true;
    else
      return false;
    
  } 
}

/*
 * Function that combines two bounding boxes by finding the min/max from each
 *  respective box and returns the new box
 * Used to create the BoundBox's for internal tree nodes
 */
BoundBox bvhNode::combine(BoundBox one, BoundBox two) {
  Vector3f min, max;
  float temp1, temp2;
  
  min = Vector3f( std::min(one.min.x(), two.min.x()),
		  std::min(one.min.y(), two.min.y()),
		  std::min(one.min.z(), two.min.z()));

  max = Vector3f( std::max(one.max.x(), two.max.x()),
		  std::max(one.max.y(), two.max.y()),
		  std::max(one.max.z(), two.max.z()));

  return BoundBox(min, max, NULL, Matrix4f::Identity());
}
 
