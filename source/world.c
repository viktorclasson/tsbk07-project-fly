#include "world.h"

// Models
Model *terrain, *skybox, *tm;

// Textures
GLuint tex1, tex2, treetexleaf;
GLuint skytex;
TextureData ttex, treetexbark; // terrain

// Shader programs
GLuint sky_program, tree_program, terrain_program;

// Variables for knowing when to draw new terrain
GLfloat x_old,z_old;

// Projection matrix, not sure why it doesn't work without it...
GLfloat projMatrix[16];

// For debugging, should be inside return_height
int x_heightmap;
int z_heightmap;
int x_center;
int z_center;

void World_Init(Point3D* camera_position, Point3D* camera_look)
{
  // Init some variables
  x_old=camera_position->x;
  z_old=camera_position->z;
  //	Infinite frustum
  frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 10000000000000000.0, projMatrix);
  // Debug
  printf("%f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n", 
	 projMatrix[0],projMatrix[1], projMatrix[2],projMatrix[3], 
	 projMatrix[4],projMatrix[5], projMatrix[6],projMatrix[7],
	 projMatrix[8],projMatrix[9], projMatrix[10],projMatrix[11],
	 projMatrix[12],projMatrix[13], projMatrix[14],projMatrix[15]);
  
  // Sky
  skybox = LoadModelPlus("objects/skybox.obj");
  sky_program = loadShaders("shaders/skybox.vert","shaders/skybox.frag");
  glUseProgram(sky_program);
  LoadTGATextureSimple("textures/SkyBox512.tga", &skytex);
  glUniformMatrix4fv(glGetUniformLocation(sky_program, "projMatrix"), 1, GL_TRUE, projMatrix);
  
  // Trees
  tree_program=loadShaders("shaders/tree.vert","shaders/tree.frag");
  glUseProgram(tree_program);
  glUniform1i(glGetUniformLocation(tree_program, "texbark"), 1); // Texture unit 0
  LoadTGATexture("textures/conc.tga", &treetexbark);
  //glUniform1i(glGetUniformLocation(tree_program, "texleaf"), 2); // Texture unit 1
  //LoadTGATextureSimple("objects/tree 1a - 1b/leafs1.tga", &treetexleaf);
  
  // Terrain
  terrain_program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
  glUseProgram(terrain_program);
  printError("init shader");

  // Is this really needed here? /Fredrik
  // It don't work without it. /Viktor
  glUniformMatrix4fv(glGetUniformLocation(terrain_program, "projMatrix"), 1, GL_TRUE, projMatrix);

  glUniform1i(glGetUniformLocation(terrain_program, "tex"), 0); // Texture unit 0
  LoadTGATextureSimple("textures/grass.tga", &tex1);
  
  // Load terrain data
  LoadTGATexture("textures/terrain.tga", &ttex);
  tm = GenerateTerrain(&ttex);
}

void World_Draw(Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix, Point3D* position)
{
    GLfloat modelView[16];
	
	printError("pre display");
	
	// Skybox
	GLfloat sky_cameraMatrix[16];
	GLfloat sky_mdlMatrix[16];
	T(camera_position->x,camera_position->y,camera_position->z,sky_mdlMatrix);
	lookAt(camera_position,camera_look,0,1,0,sky_cameraMatrix);
	sky_cameraMatrix[3] = 0;
	sky_cameraMatrix[7] = -0.5;
	sky_cameraMatrix[11] = 0;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(sky_program);
	glBindTexture(GL_TEXTURE_2D, skytex);
	glUniform1i(glGetUniformLocation(sky_program, "texUnit"), 0); // Texture unit 1
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "camMatrix"), 1, GL_TRUE, sky_cameraMatrix);
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "mdlMatrix"), 1, GL_TRUE, sky_mdlMatrix);
	DrawModel(skybox, sky_program, "in_Position", "in_Normal", "inTexCoord");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//printf("x: %f, z: %f\n",camera_position.x,camera_position.z);
	


	// Build matrix
	
	//Point3D camera_position = {0, 5, 8};
	//Point3D camera_look = {2, 0, 2};

	
	/*
	//Har kommenterat bort allt som har med kamera att g�ra s� f�r vi s�tta tillbaka den funktionaliteten steg f�r steg sen
	if(camera_position->y - 5 < calc_object_ycoord(camera_position->x, camera_position.z))
	{
	  camera_position.y = calc_object_ycoord(camera_position.x, camera_position.z) + 5;
	}
	*/
	
	//tm = GenerateTerrain(&ttex,&camera_position);

	//GenerateHeightmap(&Heightmap);

	// Create camera matrix
	//lookAt(&camera_position,&camera_look,0,1,0,camMatrix);
	
	// Terrain program
	glUseProgram(terrain_program);
	
	IdentityMatrix(modelView);
	glUniformMatrix4fv(glGetUniformLocation(terrain_program, "mdlMatrix"), 1, GL_TRUE, modelView);
	glUniformMatrix4fv(glGetUniformLocation(terrain_program, "camMatrix"), 1, GL_TRUE, camMatrix);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, terrain_program, "inPosition", "inNormal", "inTexCoord");
}

GLfloat World_GetHeight(GLfloat x, GLfloat z)
{
    GLfloat y = 0;
    return y;
}
