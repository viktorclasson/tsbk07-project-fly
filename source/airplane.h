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

void Airplane_Init(void);
void Airplane_Keyboard(Point3D &yawRate, Point3D &pitchRate, Point3D &rollRate);
void Airplane_Draw(GLfloat* camMatrix, GLfloat* projMatrix, GLfloat* mdlMatrix);

#endif