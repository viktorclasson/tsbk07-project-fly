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

// Camera settings; first or third person view
GLuint firstPersonView;

// Airplane variables
GLfloat yawRate, pitchRate, rollRate, velocity, thrust;
Point3D position, forward, up, right;

// Camera variables (needed for world_display, letting them stay for now)
Point3D camera_position, camera_look;

// Flag for signaling reset of game
GLuint resetFlag;

#include "airplane.h"
#include "camera.h"
#include "dynamics.h"
#include "world.h"
#include "game.h"

//Fï¿œr debugging endast!
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

void printVector(Point3D *v)
{
	printf("x: %f, y: %f, z: %f \n", v->x, v->y, v->z);
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
	Airplane_Init(&thrust, &yawRate, &pitchRate, &rollRate, &firstPersonView, &resetFlag);
	
	// Camera
	Camera_Init(firstPersonView, &forward, &up, &position, velocity, &camera_position, &camera_look, camMatrix);
	
	// Terrain and skybox
	World_Init(&camera_position, &camera_look);

	// Init game
	Game_Init();
	
	// Projection, skall ej användas längre, tas bort när alla beroende funktioner är fixade
	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projMatrix);
	
	printError("init arrays");	
}

void display(void)
{
    // Only Debugging!!!
    //printf("POS: x: %f, y: %f, z: %f    LOOK: x: %f, y: %f, z: %f\n", camera_position.x, camera_position.y, camera_position.z, camera_look.x, camera_look.y, camera_look.z);
    //--------------------
    
	if(Game_HitGround())
	{
	  // Check keyboard for reset
	  Airplane_Keyboard(&thrust, &yawRate, &pitchRate, &rollRate, &firstPersonView, &resetFlag);
	  Game_Over();
	  
	  if(resetFlag == 1)
	  {
	    Game_Reset(&forward, &up, &right, &position, &velocity, &thrust, &yawRate, &pitchRate, &rollRate, &firstPersonView, &resetFlag, &camera_position, &camera_look, camMatrix);
	  }
	}
	else
	{
	  // Get user input
	  Airplane_Keyboard(&thrust, &yawRate, &pitchRate, &rollRate, &firstPersonView, &resetFlag);

	  // Update airplane dynamics
	  Dynamics_CalcRot(yawRate, pitchRate, rollRate, &forward, &up, &right);
	  Dynamics_CalcPos(thrust, &forward, &velocity, &position);
	
	  // Commented for Debugging: Using mouse to look instead
	  // Update camera
	  Camera_Update(firstPersonView, &forward, &up, &position, velocity, &camera_position, &camera_look, camMatrix);

	  printError("pre display");

	  // clear the screen
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  
	  // Draw ground and skybox
	  World_Draw(&camera_position, &camera_look, camMatrix, &position, &up, firstPersonView);
	  
	  // Check for collision
	  Game_CollisionDetection(&position, &forward, &up, &right);
	  
	  // The game loop
	  Game_Loop(position);

	  
	  // Draw airplane
	  Airplane_Draw(&forward, &up, &right, &position, camMatrix);
	  
	  // Draw target
	  Game_DrawTarget(camMatrix);
	  

	  // Reset game?
	  if(resetFlag == 1)
	  {
	    Game_Reset(&forward, &up, &right, &position, &velocity, &thrust, &yawRate, &pitchRate, &rollRate, &firstPersonView, &resetFlag, &camera_position, &camera_look, camMatrix);
	  }
	}
	
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
