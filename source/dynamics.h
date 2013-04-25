#ifndef DYNAMICS
#define DYNAMICS

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include "utils/VectorUtils2.h"

void Dynamics_Init(Point3D* forward, Point3D* up, Point3D* right, 
		   Point3D* position, GLfloat* velocity);

void Dynamics_CalcRot(GLfloat yawRate, GLfloat pitchRate, GLfloat rollRate, 
		 Point3D* forward, Point3D* up, Point3D* right);
		 
void Dynamics_CalcPos(GLfloat thrust, Point3D* forward, GLfloat* velocity, Point3D* position);

void Dynamics_CalcMdlMatrix(Point3D* forward, Point3D* up, Point3D* right, 
			    Point3D* position, GLfloat* mdlMatrix);

#endif