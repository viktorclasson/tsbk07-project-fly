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

// Generate the terrain, this function is not so big anymore since much is moved to the shader
Model* GenerateTerrain(TextureData *tex);

#endif
