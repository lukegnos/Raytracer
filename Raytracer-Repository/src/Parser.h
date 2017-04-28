#include "raycaster.h"
#include "DrawableObject.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Box.h"
#include "Scene.h"
#include <math.h>

#define PI 3.14159265

class Parser {
public:
  Parser();
  ~Parser();

  void parseFile(string infile, Scene *scene);
  void getCamera(Camera *camera);
  
  void getPlane(string line, vector<Plane *> *olist);
  void getSphere(string line, vector<DrawableObject *> *olist);
  void getTriangle(vector<DrawableObject *> *olist);
  void getBox(string sline, vector<DrawableObject *> *olist);
  void getLightSource(string line, vector<LightSource*> *lights);

  Finish getFinish(string line);
  Vector3f getPoint(string line);
  Color getColor(string line);
  
  Matrix4f getScaleMatrix(string line);
  Matrix4f getRotationMatrix(string line);
  Matrix4f getTranslationMatrix(string line);
  
  ifstream inputFile;
};
