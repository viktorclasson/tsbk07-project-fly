// Generate Terrain, generates a terrain at around a certain point.

#include "generate_terrain2.h"
#include <float.h>


Model* GenerateTerrain(TextureData *tex,Point3D *camera_position)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	float heightfactor = 7.0;
	//printf("%d %d \n",tex->width,tex->height);
	//float Heightmap[tex->height][tex->width];
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * (vertexCount+8));
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * (vertexCount+8));
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * (vertexCount+8));
	GLuint *indexArray = malloc(sizeof(GLuint) * (triangleCount+12)*3);
	Point3D u;
	Point3D v;
	
	//printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / heightfactor;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
			Heightmap[x][z]=tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / heightfactor;
// Normal vectors. You need to calculate these.
			if ((x<=0) || (z<=0) || (x >= tex->width-1) || (z >= tex->width-1))
			{
			  normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			  normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			  normalArray[(x + z * tex->width)*3 + 2] = 0.0;
			}
			else
			{
			 u.x=2; u.y=tex->imageData[(x-1 + z * tex->width) * (tex->bpp/8)] / 100.0-tex->imageData[(x+1 + z * tex->width) * (tex->bpp/8)] / 100.0; u.z=0;
			 v.x=0; v.y=tex->imageData[(x + (z-1) * tex->width) * (tex->bpp/8)] / 100.0-tex->imageData[(x + (z+1) * tex->width) * (tex->bpp/8)] / 100.0; v.z=2;
			 Normalize(&u);
			 Normalize(&v);
			 CrossProduct(&v,&u,&u);
			 normalArray[(x + z * tex->width)*3 + 0] = u.x;
			  normalArray[(x + z * tex->width)*3 + 1] = u.y;
			  normalArray[(x + z * tex->width)*3 + 2] = u.z;
			  
			}

			//printf("x: %d, z: %d, normaly: %f \n",x,z,u.y);
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / 1.0; // x;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / 1.0;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	/*
	vertexArray[(tex->width + tex->height * tex->width)*3 + 0] = FLT_MAX;
	vertexArray[(tex->width + tex->height * tex->width)*3 + 1] = 0;
	vertexArray[(tex->width + tex->height * tex->width)*3 + 2] = FLT_MAX;
	
	vertexArray[(tex->width + tex->height * tex->width)*3 + 3] = FLT_MAX;
	vertexArray[(tex->width + tex->height * tex->width)*3 + 4] = 0;
	vertexArray[(tex->width + tex->height * tex->width)*3 + 5] = -FLT_MAX;
	
	indexArray[(tex->width + tex->height * tex->width)*6 + 0] = tex->width + tex->height * tex->width;
	indexArray[(tex->width + tex->height * tex->width)*6 + 1] = tex->width + tex->height * tex->width+1;
	indexArray[(tex->width + tex->height * tex->width)*6 + 2] = tex->width + tex->height * tex->width-1;
	*/
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

void GenerateHeightmap(float *temp)
{
  temp = *Heightmap;
}