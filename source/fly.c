// I believe I can fly, a flight simulator

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif

#include "utils/GL_utilities.h"
#include "utils/VectorUtils2.h"
#include <math.h>

// Matrices to send to shaders
GLfloat camMatrix[16], projMatrix[16]; //projMatrix skall ej användas längre, tas bort när alla beroende funktioner är fixade

// Airplane variables
GLfloat yawRate, pitchRate, rollRate, velocity, thrust;
Point3D position, forward, up, right;
GLfloat front, back, leftWing, rightWing, bottom, top;

// Camera variables (needed for world_display, letting them stay for now)
Point3D camera_position, camera_look;

// Game flow flags
int hitGround;

#include "airplane.h"
#include "camera.h"
#include "dynamics.h"
#include "world.h"


//Fï¿½r debugging endast!
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
	
	lookAt(&camera_position,&camera_look,0,1,0,camMatrix);
	
	x_click = x;
	y_click = y;
}

void mouseclick(int button, int state, int x, int y)
{
	x_click = x;
	y_click = y;
}
//____________________________







void collisionDetection(Point3D* position, Point3D* forward, Point3D *up, Point3D* right)
{
  Point3D edgePoint;
  GLfloat ground;
  
  // Check bottom of plane
  ScalarMult(up, bottom, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {printf("Bottom hit ground! \n"); hitGround = 1};
  
  // Check top of plane
  ScalarMult(up, top, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {printf("Top hit ground! \n"); hitGround = 2};
  
  // Check front of plane
  ScalarMult(forward, front, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {printf("Front hit ground! \n"); hitGround = 3};
  
  // Check back of plane
  ScalarMult(forward, back, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {printf("Back hit ground! \n"); hitGround = 4};
  
  // Check front of plane
  ScalarMult(right, rightWing, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {printf("Right wing hit ground! \n"); hitGround = 5};
  
  // Check back of plane
  ScalarMult(right, leftWing, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {printf("Left wing ground! \n"); hitGround = 6};
 
}

void init(void)
{	
	dumpInfo();

	// GL inits
	glClearColor(1,1,1,0);;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	printError("GL inits");
	


	// Airplane
	Dynamics_Init(&forward, &up, &right, &position, &velocity);
	Airplane_Init(&thrust, &yawRate, &pitchRate, &rollRate);
	Airplane_FindEdges(&front, &back, &leftWing, &rightWing, &top, &bottom);
	
	// Camera
	Camera_Init(&forward, &up, &position, velocity, &camera_position, &camera_look, camMatrix);
	
	// Terrain and skybox
	World_Init(&camera_position, &camera_look);


	// Projection, skall ej användas längre, tas bort när alla beroende funktioner är fixade
	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projMatrix);
	
	printError("init arrays");	
}

void display(void)
{
    // Only Debugging!!!
    //printf("POS: x: %f, y: %f, z: %f    LOOK: x: %f, y: %f, z: %f\n", camera_position.x, camera_position.y, camera_position.z, camera_look.x, camera_look.y, camera_look.z);
    //--------------------
    
    
	// Get user input
	Airplane_Keyboard(&thrust, &yawRate, &pitchRate, &rollRate);

	// Update airplane dynamics
	Dynamics_CalcRot(yawRate, pitchRate, rollRate, &forward, &up, &right);
	Dynamics_CalcPos(thrust, &forward, &velocity, &position);
	
	// Commented for Debugging: Using mouse to look instead
	// Update camera
	Camera_Update(&forward, &up, &position, velocity, &camera_position, &camera_look, camMatrix);

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw ground and skybox
	World_Draw(&camera_position, &camera_look, camMatrix, &position, &up, &right, &forward);
	
	// Check for collision
	collisionDetection(&position, &forward, &up, &right);
	
	// Draw airplane
	Airplane_Draw(&forward, &up, &right, &position, camMatrix);
	
	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20.0, &OnTimer, value);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("I believe I can fly");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	//glutSpecialFunc();
	glutTimerFunc(20, &OnTimer, 0);
	
	glutMouseFunc(&mouseclick);
	glutMotionFunc(&mousemove);

	glutMainLoop();
	exit(0);
}
