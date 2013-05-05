#include "world.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>

// Models
Model *terrain, *skybox, *tm, *tree;

// Textures
GLuint tex1, tex2, treetexleaf, treetexbark;
GLuint skytex;
TextureData ttex; // terrain

// Shader programs
GLuint sky_program, tree_program, terrain_program;

// Variables for knowing when to draw new terrain
GLfloat x_old,z_old;

// Projection matrix, not sure why it doesn't work without it...
GLfloat worldprojMatrix[16];

// For debugging, should be inside return_height
int x_heightmap;
int z_heightmap;
int x_center;
int z_center;

float bottom_tree;

// Returns the bottom of a model.
float get_bottom(Model *m)
{
        int i;
        float miny = 1e10;
        printf("numVertices: %d \n",m->numVertices);
        for (i = 0; i < m->numVertices; i++)
        {
                if (m->vertexArray[3 * i+1] < miny) miny = m->vertexArray[3 * i+1];
        }

        return miny;
}

void World_Init(Point3D* camera_position, Point3D* camera_look)
{
  // Init some variables
  x_old=camera_position->x;
  z_old=camera_position->z;
  //    Infinite frustum
  frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 10000000000000000.0, worldprojMatrix);
  // Debug
  printf("%f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n",
         worldprojMatrix[0],worldprojMatrix[1], worldprojMatrix[2],worldprojMatrix[3],
         worldprojMatrix[4],worldprojMatrix[5], worldprojMatrix[6],worldprojMatrix[7],
         worldprojMatrix[8],worldprojMatrix[9], worldprojMatrix[10],worldprojMatrix[11],
         worldprojMatrix[12],worldprojMatrix[13], worldprojMatrix[14],worldprojMatrix[15]);
         
 
  // Sky
  skybox = LoadModelPlus("objects/skybox.obj");
  sky_program = loadShaders("shaders/skybox.vert","shaders/skybox.frag");
  glUseProgram(sky_program);
  LoadTGATextureSimple("textures/SkyBox512.tga", &skytex);
  glUniformMatrix4fv(glGetUniformLocation(sky_program, "projMatrix"), 1, GL_TRUE, worldprojMatrix);
 
          glUniform1i(glGetUniformLocation(sky_program, "texUnit"), 3); // Texture unit 3
	  
  // Trees
  tree = LoadModelPlus("objects/tree1.obj");
  bottom_tree = get_bottom(tree);
  printf("%f \n", bottom_tree);
  tree_program=loadShaders("shaders/tree.vert","shaders/tree.frag");//,"shaders/tree.geo");
  glUseProgram(tree_program);
  glUniform1i(glGetUniformLocation(tree_program, "texbark"), 4); // Texture unit 0
  LoadTGATextureSimple("textures/conc.tga", &treetexbark);
  //glUniform1i(glGetUniformLocation(tree_program, "texleaf"), 2); // Texture unit 1
  //LoadTGATextureSimple("objects/tree 1a - 1b/leafs1.tga", &treetexleaf);
 
  // Terrain
  terrain_program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
  glUseProgram(terrain_program);
  printError("init shader");

  // Is this really needed here? /Fredrik
  // It don't work without it. /Viktor
  glUniformMatrix4fv(glGetUniformLocation(terrain_program, "projMatrix"), 1, GL_TRUE, worldprojMatrix);

  glUniform1i(glGetUniformLocation(terrain_program, "tex"), 5); // Texture unit 0
  glUniform1i(glGetUniformLocation(terrain_program, "skybox_tex"), 1);
  LoadTGATextureSimple("textures/grass.tga", &tex1);
 
  // Load terrain data
  LoadTGATexture("textures/terrain.tga", &ttex);
  tm = GenerateTerrain(&ttex);
}

GLfloat World_GetHeight(GLfloat x, GLfloat z)
{
  // Insert random function here (which is the same random function as in the shader)
  GLfloat y = abs((sin(x/600)*sin(z/400)*100)+(5.0*sin(z/100.0-0.5)*2.0*sin(x/40.0)))-1.5;
  int integer = 70;
  if (y>integer)
    y=y*pow(y-integer+1,0.6*sin(x/1000)*sin(z/5000));
  return y;
}

void World_Draw(Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix, Point3D* position, Point3D* plane_up, GLuint firstPersonView)
{
	// For time debugging
	struct timespec t1,t2,t3,t4;
	// For time critical debugging
	clock_gettime(CLOCK_REALTIME, &t3);
  
	GLfloat modelView[16];
       
        printError("pre display");
       
        // Skybox
        GLfloat sky_cameraMatrix[16];
	//printf("First person: %d \n plane_up %f %f %f \n",firstPersonView,plane_up->x,plane_up->y,plane_up->z);
	//printf("Absolute value: %f \n", sqrt(powf(plane_up->x,2) + powf(plane_up->y,2) + powf(plane_up->z,2)));
	Point3D yhat = {0,1,0};
	//printf("Yhat: %f %f %f\n",yhat.x,yhat.y,yhat.z);
	if(firstPersonView == 1) // Equals to first persion view!
	{

	  //CrossProduct(&yhat,plane_up,&yhat);
	  lookAt(camera_position,camera_look,plane_up->x,plane_up->y,plane_up->z,sky_cameraMatrix);
	  OrthoNormalizeMatrix(sky_cameraMatrix);
	  MatrixMultPoint3D(sky_cameraMatrix,&yhat,&yhat);
	  Normalize(&yhat);
	  //lookAt(camera_position,camera_look,yhat.x,yhat.y,yhat.z,sky_cameraMatrix);
	  //sky_cameraMatrix[3] = -0.5*plane_up->x;
	  //sky_cameraMatrix[7] = -0.5*plane_up->y;
	  //sky_cameraMatrix[11] = -0.5*plane_up->z;
	  sky_cameraMatrix[3] = -0.5*yhat.x;
	  sky_cameraMatrix[7] = -0.5*yhat.y;
	  sky_cameraMatrix[11] = -0.5*yhat.z;
	  //sky_cameraMatrix[3] = 0.0;
	  //sky_cameraMatrix[7] = 0.0;
	  //sky_cameraMatrix[11] = 0.0;
	}
	else
	{
	  lookAt(camera_position,camera_look,0,1,0,sky_cameraMatrix);
	  sky_cameraMatrix[3] = 0.0;
	  sky_cameraMatrix[7] = -0.5;
	  sky_cameraMatrix[11] = 0.0;
	}
	
	//printf("Cam mult yhat: %f %f %f\n",yhat.x,yhat.y,yhat.z);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glUseProgram(sky_program);
	glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, skytex);

        glUniformMatrix4fv(glGetUniformLocation(sky_program, "camMatrix"), 1, GL_TRUE, sky_cameraMatrix);
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "projMatrix"), 1, GL_TRUE, worldprojMatrix);
        DrawModel(skybox, sky_program, "in_Position", "in_Normal", "inTexCoord");
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        //printf("x: %f, z: %f\n",position->x,position->z);
       
        // Tree program
        //      Inits
        glUseProgram(tree_program);
        GLfloat tree_mtwMatrix[16];
        GLfloat tree_scaleMatrix[16];
        Point3D tree_position;
	
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, treetexbark);
        glUniform1i(glGetUniformLocation(tree_program, "texbark"), 0);
       
        // 	Upload projection matrix
        glUniformMatrix4fv(glGetUniformLocation(tree_program, "projMatrix"), 1, GL_TRUE, worldprojMatrix);
       
        // 	Upload camera matrix
        glUniformMatrix4fv(glGetUniformLocation(tree_program, "camMatrix"), 1, GL_TRUE, camMatrix);
		
	// 	Drawing/placing loop
        // For time critical debugging
	clock_gettime(CLOCK_REALTIME, &t1);
	int i;
	S(60.0,60.0,60.0,tree_scaleMatrix);
	float constant_factor = 6000.0;
	//int mult_factor_x = (int)(camera_position->x/(2.0*constant_factor));
	//int mult_factor_z = (int)(camera_position->z/(2.0*constant_factor));
	int mult_factor_x = (int)(camera_position->x/constant_factor);
	int mult_factor_z = (int)(camera_position->z/constant_factor);
	Point3D viewdirection;
	for(i=200;i>0;i=i-1)
	{
	  
	  tree_position.x=2.0*constant_factor*sin((float)i/100.0-0.5);

	  tree_position.z=constant_factor*sin((float)i);
	  // Use the cameramatrix to understand what we are looking at
	  MatrixMultPoint3D(camMatrix, &tree_position, &viewdirection);
	  
	  if(viewdirection.z < 0)
	  {
	    // We are looking at the tree => do nothing
	  }
	  else
	  {
	    // We are not looking at the tree => do something smart!
	    // Something smart:
	    tree_position.x=tree_position.x+mult_factor_x*constant_factor;
	    tree_position.z=tree_position.z+mult_factor_z*constant_factor;
	    
	  }

	  tree_position.y=bottom_tree+World_GetHeight(tree_position.x,tree_position.z);

	  // Draw only if above water level
	  if(tree_position.y > 1)
	  {
		  //printf("******\nTree x: %f, z: %f \n",tree_position.x,tree_position.z);
		  //printf("Multfactor x: %d, z: %d \n",mult_factor_x,mult_factor_z);
		  //printf("Camera x: %f, z: %f\n",camera_position->x,camera_position->z);
		  T(tree_position.x,tree_position.y,tree_position.z,tree_mtwMatrix);
		  Mult(tree_mtwMatrix,tree_scaleMatrix,tree_mtwMatrix);
		  // Upload model to world matrix and draw objects
		  glUniformMatrix4fv(glGetUniformLocation(tree_program, "mdlMatrix"), 1, GL_TRUE, tree_mtwMatrix);
		  
		  //printf("Mult x: %f,y: %f, z: %f \n*******\n",viewdirection.x,viewdirection.y,viewdirection.z);

		  DrawModel(tree, tree_program, "inPosition", "inNormal", "texCoord");
	  }
	}
	// For time critical debugging
	clock_gettime(CLOCK_REALTIME, &t2);

        // Terrain program
        glUseProgram(terrain_program);
       
        IdentityMatrix(modelView);
        glUniformMatrix4fv(glGetUniformLocation(terrain_program, "mdlMatrix"), 1, GL_TRUE, modelView);
        glUniformMatrix4fv(glGetUniformLocation(terrain_program, "camMatrix"), 1, GL_TRUE, camMatrix);
	GLfloat curPos[3];
	curPos[0] = position->x;
	curPos[1] = position->y;
	curPos[2] = position->z;
	glUniform3fv(glGetUniformLocation(terrain_program, "currentPosition"), 1, curPos);
        glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, skytex);             // Bind Our Texture tex1
        DrawModel(tm, terrain_program, "inPosition", "inNormal", "inTexCoord");
        // For time critical debugging
	clock_gettime(CLOCK_REALTIME, &t4);
	
	// Debugging
	//printf("******* \n Looptime: %d ns \n Time of the rest: %d ns \n******** \n",t2.tv_nsec-t1.tv_nsec,t4.tv_nsec-t3.tv_nsec-(t2.tv_nsec-t1.tv_nsec));
}
