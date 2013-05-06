#ifndef DYNAMICS
#define DYNAMICS

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils/VectorUtils2.h"

#define pAlpha 0.90
#define rAlpha 0.90
#define yAlpha 0.90
#define maxVelocity 30 // Maxhastighet
#define thrustRespons 20 // Lutningen på hastighetskurvan, lägre värde ger brantare lutning initalt

void Dynamics_Init(Point3D* forward, Point3D* up, Point3D* right, 
		   Point3D* position, GLfloat* velocity);

void Dynamics_CalcRot(GLfloat yawInput, GLfloat pitchInput, GLfloat rollInput, 
		 Point3D* forward, Point3D* up, Point3D* right);
		 
void Dynamics_CalcPos(GLfloat thrust, Point3D* forward, GLfloat* velocity, Point3D* position);

#endif
