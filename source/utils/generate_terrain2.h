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

// Heightmap, the heightmap is the height of objects that are around the point defined in Heightmap[1-2][513]
float Heightmap[512][513];

Model* GenerateTerrain(TextureData *tex, Point3D *camera_position);

void GenerateHeightmap(float *temp);

#endif
