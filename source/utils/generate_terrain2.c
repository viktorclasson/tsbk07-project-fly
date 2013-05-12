// Generate Terrain, generates a terrain at around a certain point.

#include "generate_terrain2.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <math.h>

// The terrain will be built like this:
// Change this value to maximize performance
int mapvertexsize = 1024;

Model* GenerateTerrain(TextureData *tex)
{
	// Define the size of the terrain and allocate memory
	int vertexCount = mapvertexsize * mapvertexsize;
	int triangleCount = (mapvertexsize-1) * (mapvertexsize-1) * 2;

	int n_x,n_z;
	float x,z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * (vertexCount+8));
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * (vertexCount+8));
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * (vertexCount+8));
	GLuint *indexArray = malloc(sizeof(GLuint) * (triangleCount+12)*3);
	
	// Loop through all vertices
	for(n_x=0; n_x < mapvertexsize;n_x++)
		for(n_z=0;n_z < mapvertexsize;n_z++)
		{
			// Generate x and y in the interval -mapvertexsize/2 to mapvertexsize/2
			x=n_x-mapvertexsize/2;
			z=n_z-mapvertexsize/2;

			// More detail in the center of the map
			x=x-exp(-20*x/(mapvertexsize))+exp(20*x/(mapvertexsize));
			z=z-exp(-20*z/(mapvertexsize))+exp(20*z/(mapvertexsize));
			//printf("x: %f, z: %f \n n_x: %d, n_z: %d \n ",x,z,n_x,n_z);

			// Vertex array
			vertexArray[(n_x + n_z * mapvertexsize)*3 + 0] = x / 1.0;
			// The y-component will be generated in the shader
			vertexArray[(n_x + n_z * mapvertexsize)*3 + 1] = 0;
			vertexArray[(n_x + n_z * mapvertexsize)*3 + 2] = z / 1.0;
			// Normal vectors, the normal will be calculated in the shader
			normalArray[(n_x + n_z * mapvertexsize)*3 + 0] = 0.0;
			normalArray[(n_x + n_z * mapvertexsize)*3 + 1] = 1.0;
			normalArray[(n_x + n_z * mapvertexsize)*3 + 2] = 0.0;
			// Texture coordinates, these are also created in the shader
			texCoordArray[(n_x + n_z * mapvertexsize)*2 + 0] = (float)x / 1.0;
			texCoordArray[(n_x + n_z * mapvertexsize)*2 + 1] = (float)z / 1.0;
			// Connect the vertexes
			if ((n_z < mapvertexsize-1) && (n_x < mapvertexsize-1))
			{
				// Triangle 1
				indexArray[(n_x + n_z * (mapvertexsize-1))*6 + 0] = n_x + n_z * mapvertexsize;
				indexArray[(n_x + n_z * (mapvertexsize-1))*6 + 1] = n_x + (n_z+1) * mapvertexsize;
				indexArray[(n_x + n_z * (mapvertexsize-1))*6 + 2] = n_x+1 + n_z * mapvertexsize;
				// Triangle 2
				indexArray[(n_x + n_z * (mapvertexsize-1))*6 + 3] = n_x+1 + n_z * mapvertexsize;
				indexArray[(n_x + n_z * (mapvertexsize-1))*6 + 4] = n_x + (n_z+1) * mapvertexsize;
				indexArray[(n_x + n_z * (mapvertexsize-1))*6 + 5] = n_x+1 + (n_z+1) * mapvertexsize;
			}
		}
	// End of terrain generation

	// Create Model and upload to GPU:
	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}
/*
void GenerateHeightmap(float *temp)
{
  temp = *Heightmap;
}
*/
