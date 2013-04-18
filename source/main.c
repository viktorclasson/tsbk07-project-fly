// I believe I can fly, a flight simulator

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif

#include "utils/GL_utilities.h"
#include "utils/VectorUtils2.h"
#include "utils/loadobj.h"
#include "utils/LoadTGA2.h"
#include "utils/generate_terrain2.h"

#include <math.h>

GLfloat projectionMatrix[16];

// Vectors
Point3D y_hat;
Point3D camera_look;
Point3D camera_position; 

// Starting position
GLfloat xp = 120;
GLfloat yp = 20;
GLfloat zp = 120;


// Starting look
GLfloat xl = 100;
GLfloat yl = 0;
GLfloat zl = 100;


GLfloat camMatrix[16];
// The last row is for central position
float Heightmap[512][513];

// vertex array object
Model *m, *m2, *tm, *plane, *tree;
// Skybox
Model *skybox;
// Reference to shader program
GLuint program;
GLuint sky_program;
GLuint tree_program;

// Texture
GLuint tex1, tex2, treetexbark, treetexleaf;
GLuint skytex;
TextureData ttex; // terrain


float animation;
float animation_speed;
float bottom_plane;
float bottom_tree;
GLfloat x_old,z_old;

void printVector(Point3D *v)
{
	printf("x: %f, y: %f, z: %f \n", v->x, v->y, v->z);
}

void init_objects()
{
  plane = LoadModelPlus("objects/space_frigate_6.obj"); 
  tree = LoadModelPlus("objects/tree1.obj");
  skybox = LoadModelPlus("objects/skybox.obj");
}

 float get_bottom(Model *m)
{
	int i;
	float miny = 1e10;
	printf("numVertices: %d",m->numVertices);
	for (i = 0; i < m->numVertices; i++)
	{
		if (m->vertexArray[3 * i+1] < miny) miny = m->vertexArray[3 * i+1];
	}

	return miny;
}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 600.0, projectionMatrix);

	// Init some variables
	y_hat.x = 0; y_hat.y = 1; y_hat.z = 0;
	camera_look.x = xl; camera_look.y = yl; camera_look.z = zl; 
	camera_position.x = xp; camera_position.y = yp; camera_position.z = zp;
	x_old=xp;
	z_old=zp;
	animation_speed = 0.1;
	
	// Load and compile shader
	sky_program = loadShaders("shaders/skybox.vert","shaders/skybox.frag");
	glUseProgram(sky_program);
	
	// Sky
	LoadTGATextureSimple("textures/SkyBox512.tga", &skytex);	
	
	// Trees
	
	tree_program=loadShaders("shaders/tree.vert","shaders/tree.frag");
	glUseProgram(tree_program);
	glUniform1i(glGetUniformLocation(tree_program, "texbark"), 1); // Texture unit 0
	LoadTGATexture("objects/tree 1a - 1b/bark1.tga", &treetexbark);
	//glUniform1i(glGetUniformLocation(tree_program, "texleaf"), 2); // Texture unit 1
	//LoadTGATextureSimple("objects/tree 1a - 1b/leafs1.tga", &treetexleaf);
	
	
	// Terrain
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("textures/grass.tga", &tex1);
	
	init_objects();
	
	// Load terrain data
	
	LoadTGATexture("textures/terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex,&camera_position);

	GenerateHeightmap(&Heightmap);

	bottom_plane = get_bottom(plane);
	bottom_tree = get_bottom(tree);
	printError("init terrain");
	

}
// For debugging, should be inside return_height
int x_heightmap;
int z_heightmap;
int x_center;
int z_center;

float return_height(int pos_x, int pos_z)
{
  x_center = (int)Heightmap[1][513];
  z_center = (int)Heightmap[2][513];
  x_heightmap = pos_x-x_center+256;
  z_heightmap = pos_z-z_center+256;

  if (!(x_heightmap < 0) && (x_heightmap < 513) && !(z_heightmap < 0) && (z_heightmap < 513))
  { 
   return Heightmap[x_heightmap][z_heightmap];
  }
  else
  {
    printf("Error: value out of bounds for Heightmap. x: %d z: %d \n",x_heightmap,z_heightmap);
    return 0.0;
  }
}

float calc_object_ycoord(float pos_x, float pos_z)
{
  	float x_frac = pos_x - floor(pos_x);
	float z_frac = pos_z - floor(pos_z);
	int upper_triangle;
	float ycoord;
	
	if(pos_x >= 0 && pos_z >= 0 && pos_x < 512 && pos_z < 512)
	{
	  if(1-z_frac > x_frac)
	  {
		  upper_triangle = 1; 
	  }
	  else
	  {
		  upper_triangle = 0; 
	  }
    
	  if(upper_triangle)
	  {
	    float ycoord1=return_height((int)floor(pos_x),(int)floor(pos_z));
	    float ycoord2=return_height((int)floor(pos_x),(int)ceil(pos_z));
	    float ycoord3=return_height((int)ceil(pos_x),(int)floor(pos_z));
	    Point3D p1 = {0, ycoord1, 0};
	    Point3D p2 = {0, ycoord2, 1};
	    Point3D p3 = {1, ycoord3, 0};
	    Point3D v, u, n;
	    VectorSub(&p1, &p2, &v);
	    VectorSub(&p1, &p3, &u);
	    CrossProduct(&v, &u, &n);
	    Normalize(&n);
	    float d = - n.y*ycoord1;
	    ycoord = (-d - n.x*x_frac - n.z*z_frac)/n.y;
	  }
	  else
	  {
	    float ycoord1=return_height((int)ceil(pos_x),(int)ceil(pos_z));
	    float ycoord2=return_height((int)floor(pos_x),(int)ceil(pos_z));
	    float ycoord3=return_height((int)ceil(pos_x),(int)floor(pos_z));
	    Point3D p1 = {1, ycoord1, 1};
	    Point3D p2 = {0, ycoord2, 1};
	    Point3D p3 = {1, ycoord3, 0};
	    Point3D v, u, n;
	    VectorSub(&p1, &p2, &v);
	    VectorSub(&p1, &p3, &u);
	    CrossProduct(&u,&v,&n);
	    Normalize(&n);
	    float d = - n.y*ycoord1 - n.x*1 - n.z*1;
	    ycoord = (-d - n.x*x_frac - n.z*z_frac)/n.y;
	  }
	}
	else
	{
	 ycoord = 0.0; 
	}
	
	
	return ycoord;
  
}

void draw_object()
{
	// Program
	// Skapa mtwMatrix
	GLfloat mtwMatrix[16], uppMatrix[16], rotMatrix[16], placeMatrix[16];
	T(10,0,0,uppMatrix);
	Ry(animation/100,rotMatrix);
	Mult(rotMatrix,uppMatrix,mtwMatrix);
	T(100,0,100,placeMatrix);
	Mult(placeMatrix,mtwMatrix,mtwMatrix);
	
	Point3D position = {0.0,bottom_plane,0.0};	
	MatrixMultPoint3D(mtwMatrix,&position,&position);
	
	float ycoord;
	ycoord = calc_object_ycoord(position.x, position.z)+20;
	
	//printf("%f \n",ycoord);
	
	//ycoord=Heightmap[2][2];
	T(0,ycoord,0,placeMatrix);
	Mult(placeMatrix,mtwMatrix,mtwMatrix);
  
	// Upload projection matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	
	// Upload camera matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);
	
	// Upload model to world matrix and draw objects
  	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mtwMatrix);
 	DrawModel(plane, program, "inPosition", "inNormal", "texCoord");
	
	// Trees
	position.y=bottom_tree;
	position.x=0.0; 
	position.z=0.0;
	T(150,0,50,mtwMatrix);
	MatrixMultPoint3D(mtwMatrix,&position,&position);
	ycoord = calc_object_ycoord(position.x, position.z);
	T(0,ycoord,0,placeMatrix);
	Mult(placeMatrix,mtwMatrix,mtwMatrix);

	printf("ycoord %f, Position %f %f \n",ycoord,position.x, position.z);
	
	glBindTexture(GL_TEXTURE_2D, treetexbark);
	glUniform1i(glGetUniformLocation(tree_program, "texbark"), 0);
	
	// Upload projection matrix
	glUniformMatrix4fv(glGetUniformLocation(tree_program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	
	// Upload camera matrix
	glUniformMatrix4fv(glGetUniformLocation(tree_program, "camMatrix"), 1, GL_TRUE, camMatrix);
	
	// Upload model to world matrix and draw objects
  	glUniformMatrix4fv(glGetUniformLocation(tree_program, "mdlMatrix"), 1, GL_TRUE, mtwMatrix);
	
	DrawModel(tree, tree_program, "inPosition", "inNormal", "texCoord");
	

}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void keyboard()
{
	Point3D move_dir;
	GLfloat scale = 0.5;
	  
	if(keyIsDown('w')) // up
	{
		VectorSub(&camera_look, &camera_position, &move_dir);
		Normalize(&move_dir);
		ScalarMult(&move_dir, scale, &move_dir);;
		VectorAdd(&camera_look, &move_dir, &camera_look);
		VectorAdd(&camera_position, &move_dir, &camera_position);
	}
	else if (keyIsDown('s')) //down
	{
		VectorSub(&camera_look, &camera_position, &move_dir);
		Normalize(&move_dir);
		ScalarMult(&move_dir, -scale, &move_dir);
		VectorAdd(&camera_look, &move_dir, &camera_look);
		VectorAdd(&camera_position, &move_dir, &camera_position);
	}
	else if (keyIsDown('a')) // left
	{
		VectorSub(&camera_look, &camera_position, &move_dir);
		Normalize(&move_dir);
		ScalarMult(&move_dir, scale, &move_dir);
		CrossProduct(&y_hat, &move_dir, &move_dir);
		VectorAdd(&camera_look, &move_dir, &camera_look);
		VectorAdd(&camera_position, &move_dir, &camera_position);
	}
	else if(keyIsDown('d')) // right
	{
		VectorSub(&camera_look, &camera_position, &move_dir);
		Normalize(&move_dir);
		ScalarMult(&move_dir, -scale, &move_dir);
		CrossProduct(&y_hat, &move_dir, &move_dir);
		VectorAdd(&camera_look, &move_dir, &camera_look);
		VectorAdd(&camera_position, &move_dir, &camera_position);
	}
	else if(keyIsDown('r')) // reset
	{
		camera_position.x = xp; camera_position.y = yp; camera_position.z = zp;
		camera_look.x = xl; camera_look.y = yl; camera_look.z = zl;
	}
	else if(keyIsDown('+'))
	{
	    animation_speed += 0.05;
	}
	else if(keyIsDown('-'))
	{
	    animation_speed -= 0.05;
	}
}

int x_click;
int y_click;

void mousemove(int x, int y)
{
	Point3D position_to_look;
	Point3D look_up;
	GLfloat phi_scale = 100;
	GLfloat theta_scale = 30;
	GLfloat rot_matrix[16];
	
	// Update x and z in camera_look (look around)
	VectorSub(&camera_look, &camera_position, &position_to_look);
	Ry((GLfloat)(x-x_click)/phi_scale, rot_matrix);
	MatrixMultPoint3D(rot_matrix, &position_to_look, &position_to_look);
	VectorAdd(&camera_position, &position_to_look, &camera_look);
	
	// Update y in camera_look (look up and down)
	look_up.x = 0; look_up.z = 0; look_up.y = (y-y_click)/theta_scale;
	VectorAdd(&camera_look, &look_up, &camera_look);
	
	x_click = x;
	y_click = y;
}

void mouseclick(int button, int state, int x, int y)
{
	x_click = x;
	y_click = y;
}

void display(void)
{
	animation += animation_speed;
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat modelView[16];
	
	printError("pre display");
	
	// Skybox
	GLfloat sky_cameraMatrix[16];
	GLfloat sky_mdlMatrix[16];
	T(camera_position.x,camera_position.y,camera_position.z,sky_mdlMatrix);
	lookAt(&camera_position,&camera_look,0,1,0,camMatrix);
	CopyMatrix(camMatrix,sky_cameraMatrix);
	sky_cameraMatrix[3] = 0;
	sky_cameraMatrix[7] = -0.5;
	sky_cameraMatrix[11] = 0;
	
	if((camera_position.x - x_old > 50 ) || (camera_position.x - x_old < -50 ) || (camera_position.z - z_old > 50 ) || (camera_position.z - z_old < -50 ))
	{
		tm = GenerateTerrain(&ttex,&camera_position);
		x_old=camera_position.x;
		z_old=camera_position.z;
		printf("x: %f, z: %f \n",x_old,z_old);
		printf("Heightmap: x: %d, z: %d \n Center: x: %d, z: %d \n",x_heightmap,z_heightmap,x_center,z_center);
	}


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(sky_program);
	glBindTexture(GL_TEXTURE_2D, skytex);
	glUniform1i(glGetUniformLocation(sky_program, "texUnit"), 0); // Texture unit 1
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "camMatrix"), 1, GL_TRUE, sky_cameraMatrix);
	glUniformMatrix4fv(glGetUniformLocation(sky_program, "mdlMatrix"), 1, GL_TRUE, sky_mdlMatrix);
	DrawModel(skybox, sky_program, "in_Position", "in_Normal", "inTexCoord");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//printf("x: %f, z: %f\n",camera_position.x,camera_position.z);
	
	
	// Main program
	glUseProgram(program);

	// Build matrix
	
	keyboard();
	//Point3D camera_position = {0, 5, 8};
	//Point3D camera_look = {2, 0, 2};

	if(camera_position.y - 5 < calc_object_ycoord(camera_position.x, camera_position.z))
	{
	  camera_position.y = calc_object_ycoord(camera_position.x, camera_position.z) + 5;
	}
	
	//tm = GenerateTerrain(&ttex,&camera_position);

	//GenerateHeightmap(&Heightmap);

	// Create camera matrix
	lookAt(&camera_position,&camera_look,0,1,0,camMatrix);
	
	IdentityMatrix(modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	draw_object();
	
	printError("display 2");
	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (800, 800);
	glutCreateWindow ("I believe I can fly");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);
	glutMouseFunc(&mouseclick);
	glutMotionFunc(&mousemove);
	

	glutMainLoop();
	exit(0);
}
