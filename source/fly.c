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
GLfloat projMatrix[16], camMatrix[16], mdlMatrix[16];

// Airplane variables
GLfloat yawRate, pitchRate, rollRate, velocity, thrust;
Point3D position, forward, up, right;

void init(void)
{	
	dumpInfo();

	// GL inits
	glClearColor(1,1,1,0);;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	printError("GL inits");
	
	// Terrain and skybox
	World_Init();
	
	// Airplane
	Dynamics_Init(&forward, &up, &right, &velocity);
	Airplane_Init();
	
	// Camera
	Camera_Init(&forward, &up, &position, velocity, camMatrix);
	
	// Projection
	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projMatrix);
	
	printError("init arrays");	
}

void display(void)
{	
	// Get user input
	Airplane_Keyboard(&thrust, &yawRate, &pitchRate, &rollRate);

	// Update airplane dynamics
	Dynamics_CalcRot(yawRate, pitchRate, rollRate, &forward, &up, &right);
	Dynamics_CalcPos(thrust, &forward, &velocity, &position);
	Dynamics_CalcMdlMatrix(&forward, &up, &right, &position, mdlMatrix);
	
	// Update camera
	Camera_Update(&forward, &up, &position, velocity, camMatrix);

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw ground and skybox
	World_Draw(&camMatrix, &projMatrix, &position);
	
	// Check for collision
	collisionDetection(&position);
	
	// Draw airplane
	Airplane_Draw(&camMatrix, &projMatrix, &mdlMatrix);
	
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
	glutCreateWindow ("I belive I can fly");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutSpecialFunc(Airplane_specialKeys);
	glutTimerFunc(20, &OnTimer, 0);

	glutMainLoop();
	exit(0);
}