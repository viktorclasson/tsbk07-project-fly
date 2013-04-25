#ifndef CAMERA
#define CAMERA

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include "utils/VectorUtils2.h"

void Camera_Init(Point3D &forward, Point3D &up, Point3D &position, 
		 GLfloat velocity, GLfloat* camMatrix);
void Camera_Update(Point3D &forward, Point3D &up, Point3D &position, 
		 GLfloat velocity, GLfloat* camMatrix);

#endif