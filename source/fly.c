// I believe I can fly, a flight simulator

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif

#include <math.h>

#include "utils/GL_utilities.h"
#include "utils/VectorUtils2.h"

#include "airplane.h"
#include "camera.h"
#include "dynamics.h"
#include "world.h"
#include "game.h"

// Matrices to send to shaders
GLfloat camMatrix[16], projMatrix[16];

// Camera settings; first or third person view
GLuint firstPersonView;

// Airplane variables
GLfloat yawRate, pitchRate, rollRate, velocity, thrust;
Point3D position, forward, up, right;

// Camera variables
Point3D camera_position, camera_look;

// Flag for signaling reset of game
GLuint resetFlag;

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	
  // Projection matrix
  frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projMatrix);
	
  printError("init arrays");	
}

void display(void)
{
  if(Game_HitGround())
    {
      // Check keyboard for reset
      Airplane_Keyboard(&thrust, &yawRate, &pitchRate, &rollRate, &firstPersonView, &resetFlag);
      Game_Over();
      
      if(resetFlag == 1)
	{
	  Game_Reset(&forward, &up, &right, &position, &velocity, &thrust, &yawRate, &pitchRate, &rollRate, 
		     &firstPersonView, &resetFlag, &camera_position, &camera_look, camMatrix);
	}
    }
  else
    {
      // Get user input
      Airplane_Keyboard(&thrust, &yawRate, &pitchRate, &rollRate, &firstPersonView, &resetFlag);
      
      // Update airplane dynamics
      Dynamics_CalcRot(yawRate, pitchRate, rollRate, &forward, &up, &right);
      Dynamics_CalcPos(thrust, &forward, &velocity, &position);
      
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
      
      // Draw target
      Game_DrawTarget(camMatrix, &camera_position);

      // Draw airplane
      Airplane_Draw(&forward, &up, &right, &position, &camera_position, camMatrix);

      // Reset game?
      if(resetFlag == 1)
      {
	Game_Reset(&forward, &up, &right, &position, &velocity, &thrust, &yawRate, &pitchRate, &rollRate, 
		   &firstPersonView, &resetFlag, &camera_position, &camera_look, camMatrix);
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
  glutTimerFunc(20, &OnTimer, 0);
  glutMainLoop();
  exit(0);
}
