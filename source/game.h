#ifndef GAME
#define GAME

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include <time.h>
#include <math.h>

#include "utils/VectorUtils2.h"
#include "utils/GL_utilities.h"
#include "utils/loadobj.h"

#include "world.h"
#include "airplane.h"
#include "dynamics.h"
#include "camera.h"

void Game_CollisionDetection(Point3D* position, Point3D* forward,  Point3D* up, Point3D* right);
void Game_Init(void);
int Game_HitGround(void);
void Game_Over(void);
void Game_Reset(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* velocity, GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView, GLuint* resetFlag, Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix);
void Game_DrawTarget(GLfloat* camMatrix, Point3D* cameraPos);
void Game_DetectTargetHit(Point3D planePosition);
void Game_Loop(Point3D planePosition);
#endif
