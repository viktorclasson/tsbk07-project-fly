#ifndef _GENERATE_TERRAIN_
#define _GENERATE_TERRAIN_

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
#endif

#include "GL_utilities.h"
#include "VectorUtils2.h"
#include "loadobj.h"
#include "LoadTGA2.h"

//float *Heightmap[256][256]
float Heightmap[512][512];

Model* GenerateTerrain(TextureData *tex, Point3D *camera_position);

void GenerateHeightmap(float *temp);

#endif