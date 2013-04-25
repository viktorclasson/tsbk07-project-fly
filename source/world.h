#ifndef WORLD
#define WORLD

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include "utils/VectorUtils2.h"
#include "utils/GL_utilities.h"
#include "utils/loadobj.h"
#include "utils/LoadTGA2.h"

void World_Init(void);
void World_Draw(GLfloat* camMatrix, GLfloat* projMatrix, Point3D* position);
GLfloat World_GetHeight(GLfloat x, GLfloat z);

#endif