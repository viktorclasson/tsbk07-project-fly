#include "game.h"

// 
GLfloat front, back, leftWing, rightWing, bottom, top;

// Game flow flags
int hitGround, hitGroundEnable;

void Game_Init(void)
{
  Airplane_FindEdges(&front, &back, &leftWing, &rightWing, &top, &bottom);
  hitGround = 0;
  hitGroundEnable = 1;
  printf("Front: %f, Back: %f, Left: %f, Right: %f, Top: %f, Bottom: %f \n", 
	 front, back, leftWing, rightWing, top, bottom);
}

int Game_HitGround(void)
{
    return hitGround;
}

void Game_CollisionDetection(Point3D* position, Point3D* forward,  Point3D* up, Point3D* right)
{
  Point3D edgePoint;
  GLfloat ground;
  
  // Check bottom of plane
  ScalarMult(up, bottom, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {
    printf("Bottom hit ground! \n"); hitGround = 1*hitGroundEnable;
  }
  
  // Check top of plane
  ScalarMult(up, top, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {
    printf("Top hit ground! \n"); hitGround = 2*hitGroundEnable;
  }
  
  // Check front of plane
  ScalarMult(forward, front, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {
    printf("Front hit ground! \n"); hitGround = 3*hitGroundEnable;
  }
  
  // Check back of plane
  ScalarMult(forward, back, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {
    printf("Back hit ground! \n"); hitGround = 4*hitGroundEnable;
  }
  
  // Check front of plane
  ScalarMult(right, rightWing, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {
    printf("Right wing hit ground! \n"); hitGround = 5*hitGroundEnable;
  }
  
  // Check back of plane
  ScalarMult(right, leftWing, &edgePoint);
  VectorAdd(position, &edgePoint, &edgePoint);
  ground = World_GetHeight(edgePoint.x, edgePoint.z);
  if(edgePoint.y <= ground) {
    printf("Left wing hit ground! \n"); hitGround = 6*hitGroundEnable;
  }
}

void Game_Over(void)
{
 // Do something 
}
