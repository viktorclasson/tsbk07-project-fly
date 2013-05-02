#ifndef GAME
#define GAME

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include "utils/VectorUtils2.h"
#include "world.h"
#include "airplane.h"

void Game_CollisionDetection(Point3D* position, Point3D* forward,  Point3D* up, Point3D* right);
void Game_Init(void);
int Game_HitGround(void);
void Game_Over(void);
void Game_DrawBullseye(void);

#endif
