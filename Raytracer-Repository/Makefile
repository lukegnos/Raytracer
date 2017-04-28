
CC = g++
CFLAGS = -O3 -fopenmp

all : main.o Triangle.o Ray.o Finish.o LightSource.o Parser.o Sphere.o DrawableObject.o Plane.o HitObject.o Image.o Camera.o Box.o BoundBox.o bvhNode.o Scene.o
	$(CC) $(CFLAGS) -o raycast main.o Triangle.o Scene.o Ray.o Finish.o LightSource.o Parser.o Sphere.o DrawableObject.o Plane.o HitObject.o Image.o Camera.o Box.o BoundBox.o bvhNode.o 

BoundBox.o : ./src/BoundBox.cpp
	$(CC) $(CFLAGS) -o BoundBox.o -c ./src/BoundBox.cpp 

bvhNode.o : ./src/bvhNode.cpp
	$(CC) $(CFLAGS) -o bvhNode.o -c ./src/bvhNode.cpp

Box.o : ./src/Box.cpp
	$(CC) $(CFLAGS) -o Box.o -c ./src/Box.cpp

main.o : ./src/main.cpp 
	$(CC) $(CFLAGS) -o main.o -c ./src/main.cpp

Triangle.o : ./src/Triangle.cpp 
	$(CC) $(CFLAGS) -o Triangle.o -c ./src/Triangle.cpp

Scene.o : ./src/Scene.cpp 
	$(CC) $(CFLAGS) -o Scene.o -c ./src/Scene.cpp

Ray.o : ./src/Ray.cpp
	$(CC) $(CFLAGS) -o Ray.o -c ./src/Ray.cpp

Finish.o : ./src/Finish.cpp 
	$(CC) $(CFLAGS) -o Finish.o -c ./src/Finish.cpp

LightSource.o : ./src/LightSource.cpp 
	$(CC) $(CFLAGS) -o LightSource.o -c ./src/LightSource.cpp

Parser.o : ./src/Parser.cpp
	$(CC) $(CFLAGS) -o Parser.o -c ./src/Parser.cpp

Sphere.o : ./src/Sphere.cpp 
	$(CC) $(CFLAGS) -o Sphere.o -c ./src/Sphere.cpp

DrawableObject.o : ./src/DrawableObject.cpp
	$(CC) $(CFLAGS) -o DrawableObject.o -c ./src/DrawableObject.cpp

Plane.o : ./src/Plane.cpp
	$(CC) $(CFLAGS) -o Plane.o -c ./src/Plane.cpp

HitObject.o : ./src/HitObject.cpp 
	$(CC) $(CFLAGS) -o HitObject.o -c ./src/HitObject.cpp

Image.o : ./src/Image.cpp 
	$(CC) $(CFLAGS) -o Image.o -c ./src/Image.cpp

Camera.o : ./src/Camera.cpp
	$(CC) $(CFLAGS) -o Camera.o -c ./src/Camera.cpp

clean :
	rm raycast Camera.o Image.o HitObject.o Sphere.o Plane.o HitObject.o DrawableObject.o Parser.o LightSource.o Finish.o Ray.o Scene.o main.o Triangle.o Box.o
