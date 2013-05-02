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
 
  // Trees
  tree = LoadModelPlus("objects/tree1.obj");
  bottom_tree = get_bottom(tree);
  printf("%f \n", bottom_tree);
  tree_program=loadShaders("shaders/tree.vert","shaders/tree.frag");//,"shaders/tree.geo");
  glUseProgram(tree_program);
  glUniform1i(glGetUniformLocation(tree_program, "texbark"), 1); // Texture unit 0
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

  glUniform1i(glGetUniformLocation(terrain_program, "tex"), 0); // Texture unit 0
  LoadTGATextureSimple("textures/grass.tga", &tex1);
 
  // Load terrain data
  LoadTGATexture("textures/terrain.tga", &ttex);
  tm = GenerateTerrain(&ttex);
}

GLfloat World_GetHeight(GLfloat x, GLfloat z)
{
	// Insert random function here (which is the same random function as in the shader)
    // GLfloat y = abs((sin(x/600)*sin(z/400)*100));
	GLfloat y = abs((sin(x/600)*sin(z/400)*100)+(5.0*sin(z/100.0-0.5)*2.0*sin(x/40.0)))-1.5;
    return y;
}

void World_Draw(Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix, Point3D* position, Point3D* plane_up, Point3D* plane_right, Point3D* plane_forward)
{
	// For time debugging
	struct timespec t1,t2,t3,t4;
	// For time critical debugging
	clock_gettime(CLOCK_REALTIME, &t3);
  
	GLfloat modelView[16];
       
        printError("pre display");
       
        // Skybox
        GLfloat sky_cameraMatrix[16];
        GLfloat sky_mdlMatrix[16];
        T(camera_position->x,camera_position->y,camera_position->z,sky_mdlMatrix);
	//plane_up->x=0;
	//plane_up->y=1;
	//plane_up->z=0;
        lookAt(camera_position,camera_look,plane_up->x,plane_up->y,plane_up->z,sky_cameraMatrix);
        sky_cameraMatrix[3] = -0.5*plane_up->x;
        sky_cameraMatrix[7] = -0.5*plane_up->y;
        sky_cameraMatrix[11] = -0.5*plane_up->z;
	
	// Not necessary
	/*
	GLfloat Rot[16];
	Rot[0] = plane_right->x ; Rot[1] = plane_up->x; Rot[2] = plane_forward->x; Rot[3] = 0;
	Rot[4] = plane_right->y ; Rot[5] = plane_up->y; Rot[6] = plane_forward->y; Rot[7] = 0;
	Rot[8] = plane_right->z ; Rot[9] = plane_up->z; Rot[10] = plane_forward->z; Rot[11] = 0;
	Rot[12] = 0 ; Rot[13] = 0; Rot[14] = 0; Rot[15] = 1;
	Transpose(Rot,Rot);
	*/

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glUseProgram(sky_program);
        glBindTexture(GL_TEXTURE_2D, skytex);
        glUniform1i(glGetUniformLocation(sky_program, "texUnit"), 0); // Texture unit 1
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
        GLfloat tree_placeMatrix[16];
        Point3D tree_position;
	
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
	S(50.0,50.0,50.0,tree_scaleMatrix);
	float constant_factor = 1000.0;
	//int mult_factor_x = (int)(camera_position->x/(2.0*constant_factor));
	//int mult_factor_z = (int)(camera_position->z/(2.0*constant_factor));
	int mult_factor_x = (int)(camera_position->x/1000.0);
	int mult_factor_z = (int)(camera_position->z/1000.0);
	Point3D testtt;
	for(i=1;i>0;i=i-1)
	{
	  /*
	  tree_position.x=2.0*constant_factor*sin((float)i/100.0-0.5);
	  if(tree_position.x > camera_position->x+constant_factor || tree_position.x < camera_position->x-constant_factor)
	  {
		  tree_position.x = tree_position.x+constant_factor*mult_factor_x;
	  }
	  tree_position.z=constant_factor*sin((float)i);
	  if(tree_position.z > camera_position->z+constant_factor || tree_position.z < camera_position->z-constant_factor)
	  {
		  tree_position.z = tree_position.z+constant_factor*mult_factor_z;
	  }
	  */
	  tree_position.x=0;
	  tree_position.z=0;
	  
	  MatrixMultPoint3D(camMatrix, &tree_position, &testtt);
	  if(testtt.z > 0)
	  {
	    //tree_position.x=tree_position.x+300;
	    tree_position.z=tree_position.z+500;
	  }
	  if(testtt.x > 0)
	  {
	    //tree_position.x=tree_position.x+300;
	    tree_position.x=tree_position.x+500;
	  }
	  
	  
	  
	  tree_position.y=bottom_tree+World_GetHeight(tree_position.x,tree_position.z);

	  // Draw only if above water level
	  //if(tree_position.y > 1)
	  //{
		  printf("******\nTree x: %f, z: %f \n",tree_position.x,tree_position.z);
		  printf("Multfactor x: %d, z: %d \n",mult_factor_x,mult_factor_z);
		  printf("Camera x: %f, z: %f\n",camera_position->x,camera_position->z);
		  T(tree_position.x,tree_position.y,tree_position.z,tree_mtwMatrix);
		  Mult(tree_mtwMatrix,tree_scaleMatrix,tree_mtwMatrix);
		  // Upload model to world matrix and draw objects
		  glUniformMatrix4fv(glGetUniformLocation(tree_program, "mdlMatrix"), 1, GL_TRUE, tree_mtwMatrix);
		  
		  printf("Multiplication x: %f,y: %f, z: %f \n*******\n",testtt.x,testtt.y,testtt.z);

		  DrawModel(tree, tree_program, "inPosition", "inNormal", "texCoord");
	  //}
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
       
        glBindTexture(GL_TEXTURE_2D, tex1);             // Bind Our Texture tex1
        DrawModel(tm, terrain_program, "inPosition", "inNormal", "inTexCoord");
        // For time critical debugging
	clock_gettime(CLOCK_REALTIME, &t4);
	
	// Debugging
	//printf("******* \n Looptime: %d ns \n Time of the rest: %d ns \n******** \n",t2.tv_nsec-t1.tv_nsec,t4.tv_nsec-t3.tv_nsec-(t2.tv_nsec-t1.tv_nsec));
}
