#include "Parser.h"

Parser::Parser() {

}

Parser::~Parser() {

}

/*
 * Function that manages parsing the function, looping over the lines
 *  and calling the desired helper function
 */
void Parser::parseFile(string ifile, Scene *scene) {
  string line;
  string camera = "camera";
  string sphere = "sphere";
  string light = "light_source";
  string plane = "plane";
  string triangle = "triangle";

  //Open file
  inputFile.open(ifile.c_str());

  if (inputFile.is_open())
    cout << "Successfully opened [" << ifile << "] file." << endl;
  else {
    cout << "Could not open file [" << ifile << "]." << endl;
    exit(0);
  }

  //Loop over all the lines in the file, calling the necessary parse function
  while (getline(inputFile, line)) {
    
    if (line[0] == '/') //Comment
      continue;
    
    if (line.find("camera", 0) != string::npos) { //Camera
      getCamera(scene->camera);
      
    } else if (line.find("light_source", 0) != string::npos) { //Light
      getLightSource(line, scene->lights);

    } else if (line.find("plane", 0) != string::npos) { //Plane
      getPlane(line, scene->planes);
      
    } else if (line.find("sphere", 0) != string::npos) { //Sphere
      getSphere(line, scene->objects);
      
    } else if (line.find("triangle", 0) != string::npos) {//Triangle
      getTriangle(scene->objects);

    } else if (line.find("box", 0) != string::npos) { //Box
      getBox(line, scene->objects);

    }//else is empty line or unsupported as of now
  }

  inputFile.close();
}

/*
 * Function that parses the file starting at line to create a lightsource,
 *  placing it in the scene object
 */
void Parser::getLightSource(string line, vector<LightSource*> *lights) {
  size_t loc;
  Vector3f temp;
  LightSource ls;
    
  ls.location = getPoint(line);
  loc = line.find("color");
  temp = getPoint(line.substr(loc, line.size() - loc));

  ls.color.r = temp.x();
  ls.color.g = temp.y();
  ls.color.b = temp.z();

  lights->push_back(new LightSource(ls.location, ls.color));
}

/*
 * Function that parses the line to get all the Finish components
 */
Finish Parser::getFinish(string line) {
  Finish res;
  string sub;
  size_t pos;

  pos = line.find("ient");
  sub = line.substr(pos +4, line.size());
  res.amb = atof(sub.c_str());

  pos = line.find("fuse");
  sub = line.substr(pos+4, line.size());
  res.diff = atof(sub.c_str());

  pos = line.find("ular");
  sub = line.substr(pos+4, line.size());
  res.spec = atof(sub.c_str());

  pos = line.find("ness");
  sub = line.substr(pos+4, line.size());
  res.rough = atof(sub.c_str());
  
  pos = line.find("reflection");
  sub = line.substr(pos+10, line.size());
  res.refl = atof(sub.c_str());

  pos = line.find("refraction");
  sub = line.substr(pos+10, line.size());
  res.refr = atof(sub.c_str());

  pos = line.find("ior");
  sub = line.substr(pos+3, line.size());
  res.ior = atof(sub.c_str());
  
  return res;  
}

/*
 * Function that parses a the given line to create a scale matrix
 */
Matrix4f Parser::getScaleMatrix(string line) {
  Matrix4f res;
  Vector3f temp;
  temp = getPoint(line);

  res << temp.x(), 0, 0, 0,
    0, temp.y(), 0, 0,
    0, 0, temp.z(), 0,
    0, 0, 0, 1;
  
  return res;
}

/*
 * Function that parses the given line to create a rotation matrix
 */
Matrix4f Parser::getRotationMatrix(string line) {
  Matrix4f res = Matrix4f::Identity();
  Vector3f temp;
  float c, s;
  
  temp = getPoint(line);

  if (temp.x() != 0) {
    //Do x matrix
    c = cos(temp.x() * PI / 180);
    s = sin(temp.x() * PI / 180);
    
    res << 1, 0, 0, 0,
      0, c, -s, 0,
      0, s, c, 0,
      0, 0, 0, 1;
    
  } else if (temp.y() != 0) {
    //do y matrix
    c = cos(temp.y() * PI / 180);
    s = sin(temp.y() * PI / 180);
    
    res << c, 0, s, 0,
      0, 1, 0, 0,
      -s, 0, c, 0,
      0, 0, 0, 1;
    
  } else if (temp.z() != 0) {
    //do z matrix
    c = cos(temp.z() * PI / 180);
    s = sin(temp.z() * PI / 180);
    
    res << c, -s, 0, 0,
      s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1;
  }
  
  return res;
}

/*
 * Function that parses the line and creates a translation matrix
 */
Matrix4f Parser::getTranslationMatrix(string line) {
  Matrix4f res;
  Vector3f temp;
  temp = getPoint(line);

  res << 1, 0, 0, temp.x(),
    0, 1, 0, temp.y(),
    0, 0, 1, temp.z(),
    0, 0, 0, 1;

  return res;
}

/*
 * Function that parses the file, creating a Triangle object and placing it
 *  in the object list
 */
void Parser::getTriangle(vector<DrawableObject *> *olist) {
  DrawableObject *sobj;
  Triangle t;
  size_t pos;
  string num, line;
  Finish fin;
  Matrix4f temp, ctm = Matrix4f::Identity();
  bool mult = false;
  
  //Get vertices
  getline(inputFile, line);
  t.pointA = getPoint(line);
  getline(inputFile, line);
  t.pointB = getPoint(line);
  getline(inputFile, line);
  t.pointC = getPoint(line);

  //Get modifiers
  while (getline(inputFile, line)) {
    if (line.find("scale", 0) != string::npos) {
      temp = getScaleMatrix(line);
      mult = true;
      
    } else if (line.find("rotate", 0) != string::npos) {
      temp = getRotationMatrix(line);
      mult = true;
      
    } else if (line.find("translate", 0) != string::npos) {
      temp = getTranslationMatrix(line);
      mult = true;
      
    } else if (line.find("pigment", 0) != string::npos) {
      t.color = getColor(line);
      
    } else if (line.find("finish", 0) != string::npos) {
      t.finish = getFinish(line);
      
    } else
      break;

    if (mult) {
      ctm = temp * ctm;
      mult = false;
    }    
  }

  //Create object and put into list
  t.tcenter = (1/3)*(t.pointA + t.pointB + t.pointC);

  sobj = new Triangle(t.pointA, t.pointB, t.pointC,
		      t.color, t.finish, t.tcenter, ctm);
  
  olist->push_back(sobj);
}

/*
 * Function that parses the file starting at sline and creates a Sphere object
 *  placing it in the object list
 */
void Parser::getSphere(string sline, vector<DrawableObject *> *olist) {
  DrawableObject *sobj;
  Sphere s;
  size_t pos;
  string num, line;
  Finish fin;
  Matrix4f temp, ctm = Matrix4f::Identity();
  bool mult = false;
  
  //get point
  s.center = getPoint(sline);

  //get radius
  pos = sline.find_last_of(",");
  num = sline.substr(pos + 1, sline.size() - pos); 
  s.radius = atof(num.c_str());

  //get modifiers
  while (getline(inputFile, line)) {
    if (line.find("scale", 0) != string::npos) {
      temp = getScaleMatrix(line);
      mult = true;
      
    } else if (line.find("rotate", 0) != string::npos) {
      temp = getRotationMatrix(line);
      mult = true;
      
    } else if (line.find("translate", 0) != string::npos) {
      temp = getTranslationMatrix(line);
      mult = true;
      
    } else if (line.find("pigment", 0) != string::npos) {
      s.color = getColor(line);
      
    } else if (line.find("finish", 0) != string::npos) {
      fin = getFinish(line);
      
    } else
      break;

    if (mult) {
      ctm = temp * ctm;
      mult = false;
    }
  }
  
  sobj = new Sphere(s.center, s.color, s.radius, fin, ctm);
  
  olist->push_back(sobj);
}

/*
 * Function that parses the file beginning at sline and creates a Box object
 *  and places it in the object list
 */
void Parser::getBox(string sline, vector<DrawableObject *> *olist) {
  DrawableObject *sobj;
  Box b;
  size_t pos;
  string num, line;
  Matrix4f temp, ctm = Matrix4f::Identity();
  bool mult = false;

  //Get Corners
  pos = sline.find_first_of(">", 0);
  pos++;
  num = sline.substr(0, pos);
  b.corner1 = getPoint(num);
  num = sline.substr(pos, sline.size() - pos);
  b.corner2 = getPoint(num);

  //Get Modifiers
  while(getline(inputFile, line)) {
    if (line.find("scale", 0) != string::npos) {
      temp = getScaleMatrix(line);
      mult = true;
      
    } else if (line.find("rotate", 0) != string::npos) {
      temp = getRotationMatrix(line);
      mult = true;
      
    } else if (line.find("translate", 0) != string::npos) {
      temp = getTranslationMatrix(line);
      mult = true;
      
    } else if (line.find("pigment", 0) != string::npos) {
      b.color = getColor(line);
      
    } else if (line.find("finish", 0) != string::npos) {
      b.finish = getFinish(line);
      
    } else
      break;

    if (mult) {
      ctm = temp * ctm;
      mult = false;
    }
  }
  
  sobj = new Box(b.corner1, b.corner2, b.finish, b.color, ctm);
  
  olist->push_back(sobj);
}

/*
 * Function that parses the file starting at sline and creates a Plane object
 *  placing it in the object list
 */
void Parser::getPlane(string sline, vector<Plane *> *olist) {
  Plane *p;
  Vector3f norm;
  size_t pos;
  string num, line;
  Matrix4f temp, ctm = Matrix4f::Identity();
  bool mult = false;
  float dist;
  Color col;
  Finish fin;
  
  //get normal
  norm = getPoint(sline);
  //get dist
  pos = sline.find_last_of(",");
  num = sline.substr(pos + 1, line.size() - pos); 
  dist = atof(num.c_str());

  //Get Modifiers
  while (getline(inputFile, line)) {
    if (line.find("scale", 0) != string::npos) {
      temp = getScaleMatrix(line);
      mult = true;
      
    } else if (line.find("rotate", 0) != string::npos) {
      temp = getRotationMatrix(line);
      mult = true;
      
    } else if (line.find("translate", 0) != string::npos) {
      temp = getTranslationMatrix(line);
      mult = true;
      
    } else if (line.find("pigment", 0) != string::npos) {
      col = getColor(line);
      
    } else if (line.find("finish", 0) != string::npos) {
      fin = getFinish(line);
      
    } else
      break;

    if (mult) {
      ctm = temp * ctm;
      mult = false;
    }
  }
  
  //put into plane object
  p = new Plane(norm, col, dist, fin, ctm);
  
  //put object into vector list
  olist->push_back(p);
}

/*
 * Parses and creates the camera object
 */
void Parser::getCamera(Camera *camera) {
  string line;

  getline(inputFile, line);
  camera->location = getPoint(line);
  getline(inputFile, line);
  camera->upVector = getPoint(line);
  getline(inputFile, line);
  camera->rightVec = getPoint(line);
  getline(inputFile, line);
  camera->lookAt = getPoint(line); 
}

/*
 * Function that creates a Vector3f object out of the <#, #, #> format
 * Assumes only one point is specified in the string
 */
Vector3f Parser::getPoint(string line) {
  float x,y,z;
  Vector3f res;
  string num;
  size_t start, end;

  start = line.find_first_of("<", 0);
  end = line.find_first_of(",", start);
  num = line.substr(start+1, end - start);
  
  x = atof(num.c_str());

  start = end + 1;
  end = line.find_first_of(",", start);
  num = line.substr(start, end - start);
  
  y = atof(num.c_str());

  start = end + 1;
  end = line.find_first_of(">", start);
  num = line.substr(start, end - start);

  z = atof(num.c_str());

  res = Vector3f(x,y,z);

  return res;
}

/*
 * Function that creates a color structure by parsing the line
 */
Color Parser::getColor(string line) {
  Color result;
  string num;
  size_t start, end;

  //Get r
  start = line.find_first_of("<", 0);
  end = line.find_first_of(",", start);
  num = line.substr(start + 1, end - start);

  result.r = atof(num.c_str());

  //get g
  start = end + 1;
  end = line.find_first_of(",", start);
  num = line.substr(start + 1, end - start);

  result.g = atof(num.c_str());
  
  //get b
  start = end + 1;
  end = line.find_first_of(",", start);
  num = line.substr(start + 1, end - start);

  result.b = atof(num.c_str());

  //get f
  start = end + 1;
  end = line.find_first_of(">", start);
  num = line.substr(start + 1, end - start);

  result.f = atof(num.c_str());
  
  return result;
}
