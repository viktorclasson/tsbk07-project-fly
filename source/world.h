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
#include "utils/generate_terrain2.h"
#include <math.h>

void World_Init(Point3D* camera_position, Point3D* camera_look);
void World_Draw(Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix, Point3D* position);
GLfloat World_GetHeight(GLfloat x, GLfloat z);

#endif
