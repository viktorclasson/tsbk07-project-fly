#ifndef AIRPLANE
#define AIRPLANE

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include "utils/VectorUtils2.h"
#include "utils/GL_utilities.h"
#include "utils/loadobj.h"
#include "utils/LoadTGA2.h"

void Airplane_Init(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate);
void Airplane_Keyboard(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate);
void Airplane_Draw(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* camMatrix, GLfloat* projMatrix);
void Airplane_CalcMatrices(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* camMatrix, GLfloat* projMatrix, GLfloat* mdlMatrix, GLfloat* normalMatrix);

#endif