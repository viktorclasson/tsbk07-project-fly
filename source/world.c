#include "world.h"

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
  //	Infinite frustum
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
  tree_program=loadShaders("shaders/tree.vert","shaders/tree.frag");
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
    GLfloat y = abs((sin(x/600)*sin(z/400)*100));
    return y;
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
	
	// Tree program
	// 	Inits
	glUseProgram(tree_program);
	GLfloat tree_mtwMatrix[16];
	GLfloat tree_scaleMatrix[16];
	GLfloat tree_placeMatrix[16];
	Point3D tree_position;
	
	tree_position.y=bottom_tree;
	tree_position.x=0.0; 
	tree_position.z=0.0;
	T(0.0,0.0,0.0,tree_mtwMatrix);
	MatrixMultPoint3D(tree_mtwMatrix,&tree_position,&tree_position);
	//T(0.0,World_GetHeight(tree_position.x,tree_position.z),0.0,tree_placeMatrix);
	//Mult(tree_placeMatrix,tree_mtwMatrix,tree_mtwMatrix);
	S(50.0,50.0,50.0,tree_scaleMatrix);
	Mult(tree_scaleMatrix,tree_mtwMatrix,tree_mtwMatrix);

	//printf("ycoord %f, Position %f %f \n",ycoord,position.x, position.z);
	
	glBindTexture(GL_TEXTURE_2D, treetexbark);
	glUniform1i(glGetUniformLocation(tree_program, "texbark"), 0);
	
	// Upload projection matrix
	glUniformMatrix4fv(glGetUniformLocation(tree_program, "projMatrix"), 1, GL_TRUE, worldprojMatrix);
	
	// Upload camera matrix
	glUniformMatrix4fv(glGetUniformLocation(tree_program, "camMatrix"), 1, GL_TRUE, camMatrix);
	
	// Upload model to world matrix and draw objects
  	glUniformMatrix4fv(glGetUniformLocation(tree_program, "mdlMatrix"), 1, GL_TRUE, tree_mtwMatrix);
	
	DrawModel(tree, tree_program, "inPosition", "inNormal", "texCoord");
	
	// Terrain program
	glUseProgram(terrain_program);
	
	IdentityMatrix(modelView);
	glUniformMatrix4fv(glGetUniformLocation(terrain_program, "mdlMatrix"), 1, GL_TRUE, modelView);
	glUniformMatrix4fv(glGetUniformLocation(terrain_program, "camMatrix"), 1, GL_TRUE, camMatrix);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, terrain_program, "inPosition", "inNormal", "inTexCoord");
}


