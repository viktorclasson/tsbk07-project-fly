// Generate Terrain, generates a terrain at around a certain point.

#include "generate_terrain2.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <math.h>




// The terrain will be built like this:
// High resolution terrain, 512*512
int mapvertexsize = 256;
int highressize=256;
int lowressize=128;

Model* GenerateTerrain(TextureData *tex)
{
	// For debugging
	struct timespec t3;

	// Define the size of the terrain and allocate memory
	int vertexCount = mapvertexsize * mapvertexsize;
	int triangleCount = (mapvertexsize-1) * (mapvertexsize-1) * 2;

	int n_x,n_z;
	float x,z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * (vertexCount+8));
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * (vertexCount+8));
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * (vertexCount+8));
	GLuint *indexArray = malloc(sizeof(GLuint) * (triangleCount+12)*3);
	
	// The highres will be stored first in the arrays
	for(n_x=0; n_x < mapvertexsize;n_x++)
		for(n_z=0;n_z < mapvertexsize;n_z++)
		{
			// Generate x and y in the interval -256 to 255
			x=n_x-mapvertexsize/2;
			z=n_z-mapvertexsize/2;

			// More detail in the center of the map
			x=x-exp(-20*x/(mapvertexsize))+exp(20*x/(mapvertexsize));
			z=z-exp(-20*z/(mapvertexsize))+exp(20*z/(mapvertexsize));
			//printf("x: %f, z: %f \n n_x: %d, n_z: %d \n ",x,z,n_x,n_z);

			// Vertex array
			vertexArray[(n_x + n_z * mapvertexsize)*3 + 0] = x / 1.0;
			/*
			if((x >= 0) && (x < tex->width) && (z >= 0) && (z < tex->width))
			{
				//clock_gettime(CLOCK_REALTIME, &t1);
				vertexArray[(n_x + n_z * highressize)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / heightfactor;
				//clock_gettime(CLOCK_REALTIME, &t2);
			}
			else
			{
			*/
				//vertexArray[(n_x + n_z * highressize)*3 + 1] = tex->imageData[(n_x + n_z * tex->width) * (tex->bpp/8)] / 30.0;
				clock_gettime(CLOCK_REALTIME, &t3);
				int bignumber = 100000000;
				srand(t3.tv_nsec);
				float randomvalue = rand()%bignumber;
				vertexArray[(n_x + n_z * highressize)*3 + 1] = randomvalue/bignumber;
				//clock_gettime(CLOCK_REALTIME, &t4);
				//printf("RAND: %f \n",randomvalue/bignumber);
			//}
			//Heightmap[n_x][n_z]=vertexArray[(n_x + n_z * highressize)*3 + 1];
			vertexArray[(n_x + n_z * highressize)*3 + 2] = z / 1.0;
			// Normal vectors
			normalArray[(n_x + n_z * highressize)*3 + 0] = 0.0;
			normalArray[(n_x + n_z * highressize)*3 + 1] = 1.0;
			normalArray[(n_x + n_z * highressize)*3 + 2] = 0.0;
			// Texture coordinates, maybe scale them differently?
			texCoordArray[(n_x + n_z * highressize)*2 + 0] = (float)x / 1.0;
			texCoordArray[(n_x + n_z * highressize)*2 + 1] = (float)z / 1.0;
			// Connect the vertexes
			if ((n_z < highressize-1) && (n_x < highressize-1))
			{
				// Triangle 1
				indexArray[(n_x + n_z * (highressize-1))*6 + 0] = n_x + n_z * highressize;
				indexArray[(n_x + n_z * (highressize-1))*6 + 1] = n_x + (n_z+1) * highressize;
				indexArray[(n_x + n_z * (highressize-1))*6 + 2] = n_x+1 + n_z * highressize;
				// Triangle 2
				indexArray[(n_x + n_z * (highressize-1))*6 + 3] = n_x+1 + n_z * highressize;
				indexArray[(n_x + n_z * (highressize-1))*6 + 4] = n_x + (n_z+1) * highressize;
				indexArray[(n_x + n_z * (highressize-1))*6 + 5] = n_x+1 + (n_z+1) * highressize;
			}
		}

	//Heightmap[1][513]=x_current;
	//Heightmap[2][513]=z_current;
	// End of terrain generation
	
	// Debugging
	// printf("******* \n Time1: %d ns \n Randomtime: %d ns \n******** \n",t2.tv_nsec-t1.tv_nsec,t4.tv_nsec-t3.tv_nsec);

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

void GenerateHeightmap(float *temp)
{
  temp = *Heightmap;
}
