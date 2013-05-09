#include "game.h"

// Edges of airplane
GLfloat front, back, leftWing, rightWing, bottom, top;

// Game flow flags
int hitGround, hitGroundEnable, hitTarget, gameRunning, gameStarted;

// Game flow varibles
time_t start, end;

// Target model
Model *target;

// Target texture
GLuint targetTex;

// Shaders
GLuint target_program;

// Target
#define numTargets 10
Point3D targetPosition[numTargets]; // Position of targets in world
Point3D currentPosition; // Current target position
GLfloat targetAngle[numTargets]; // Angle wrt the y-axis
GLfloat currentAngle; // Current target angle
GLfloat sf;
int currentTarget;
GLfloat xmax, xmin, zmax, zmin, ymax, ymin, maxRadius;

// Matrices
GLfloat projMatrix[16];

void Game_FindTargetBoundries(void)
{
  // Get boundries
  FindEdges(target, sf, &xmax, &xmin, &ymax, &ymin, &zmax, &zmin);
  maxRadius = fmax(xmax-xmin, ymax-ymin);
  maxRadius = fmax(maxRadius, zmax-zmin);
  maxRadius /= 4;

  // Calculate boundries in world
  xmax = currentPosition.x + maxRadius;
  xmin = currentPosition.x - maxRadius;
  zmax = currentPosition.z + maxRadius;
  zmin = currentPosition.z - maxRadius;
  ymax = currentPosition.y + maxRadius;
  ymin = currentPosition.y - maxRadius;
}

void Game_PlaceTargets(void)
{
  SetVector(50,140,600,&targetPosition[0]);
  targetAngle[0] = 0;
  SetVector(50,160,650,&targetPosition[1]);
  targetAngle[1] = 0.1;
  SetVector(60,160,800,&targetPosition[2]);
  targetAngle[2] = -M_PI*0.15;
  SetVector(80,150,850,&targetPosition[3]);
  targetAngle[3] = -M_PI*0.20;
  SetVector(100,150,900,&targetPosition[4]);
  targetAngle[4] = -M_PI*0.25;
  SetVector(130,150,930,&targetPosition[5]);
  targetAngle[5] = -M_PI*0.30;
  SetVector(160,160,950,&targetPosition[6]);
  targetAngle[6] = -M_PI*0.35;
  SetVector(180,160,970,&targetPosition[7]);
  targetAngle[7] = -M_PI*0.40;
  SetVector(210,150,1000,&targetPosition[8]);
  targetAngle[8] = -M_PI*0.45;
  SetVector(270,150,1000,&targetPosition[9]);
  targetAngle[9] = -M_PI*0.5;
}

void Game_Init(void)
{
  Airplane_FindEdges(&front, &back, &leftWing, &rightWing, &top, &bottom);
  
  // Init flags
  hitGround = 0;
  hitGroundEnable = 1;
  hitTarget = 0;
  currentTarget = 0;
  gameRunning = 0; 
  gameStarted = 1;

  // Init target
  target = LoadModelPlus("objects/torus.obj");
  target_program = loadShaders("shaders/target.vert", "shaders/target.frag");
  glUseProgram(target_program);
  LoadTGATextureSimple("textures/gold.tga",&targetTex);

  // Place targets  
  Game_PlaceTargets();
  currentPosition = targetPosition[currentTarget];
  currentAngle = targetAngle[currentTarget];

  // Scale factor
  sf = 5;

  Game_FindTargetBoundries();

  // Projection
  frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000000.0, projMatrix);
  
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
 
}

void Game_Complete(void)
{
	double runTime;
	runTime = difftime(end,start);
	printf("Congratulations, you've completed the flight in %.f seconds! \n",runTime);
}

void Game_Reset(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* velocity, GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView, GLuint* resetFlag, Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix)
{
  Dynamics_Init(forward, up, right, position, velocity);
  Airplane_Init(thrust, yawRate, pitchRate, rollRate, firstPersonView, resetFlag);
  Camera_Init(*firstPersonView, forward, up, position, *velocity, camera_position, camera_look, camMatrix);
  Game_Init();
}

void Game_DrawTarget(GLfloat* camMatrix, Point3D* cameraPos)
{
  GLfloat trans[16], iniRot[16], rot[16], scale[16], mdlMatrix[16];
  
  // Calculate model to world Matrix
  S(sf, sf, sf, scale);
  T(currentPosition.x, currentPosition.y, currentPosition.z, trans);
  Ry(currentAngle, rot);
  Rx(M_PI_2, iniRot);
  IdentityMatrix(mdlMatrix);
  Mult(scale, mdlMatrix, mdlMatrix);
  Mult(iniRot, mdlMatrix, mdlMatrix);
  Mult(rot, mdlMatrix, mdlMatrix);
  Mult(trans, mdlMatrix, mdlMatrix);
  


  // Upload matrices
  glUseProgram(target_program);  

  glUniformMatrix4fv(glGetUniformLocation(target_program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix);
  glUniformMatrix4fv(glGetUniformLocation(target_program, "camMatrix"), 1, GL_TRUE, camMatrix);
  glUniformMatrix4fv(glGetUniformLocation(target_program, "projMatrix"), 1, GL_TRUE, projMatrix);
  
  // Prepare camera position and upload it
  GLfloat camPos[3] = { cameraPos->x, cameraPos->y, cameraPos->z };
  glUniform3fv(glGetUniformLocation(target_program, "camera_position"), 1, camPos); 

  // Upload textures
  glUniform1i(glGetUniformLocation(target_program, "texUnit"), 0); // Texture unit 1
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, targetTex);
  
  // Draw the target
  DrawModel(target, target_program, "inPosition", "inNormal", "inTexCoord");

}

void Game_Loop(Point3D planePosition)
{
	if(gameStarted)
	{
		gameRunning = 1;
		time(&start);
		gameStarted = 0;
	}
  
	if(gameRunning){
		Game_DetectTargetHit(planePosition);
		if (hitTarget) {
			hitTarget = 0;
			printf("Passed target %d of %d!\n",currentTarget+1,numTargets);
			if(currentTarget < numTargets - 1){
				currentTarget++;
				currentTarget = currentTarget;
				currentPosition = targetPosition[currentTarget];
				currentAngle = targetAngle[currentTarget];
				Game_FindTargetBoundries();
			}
			else{
				gameRunning = 0;
				time(&end);
				currentTarget = 0;
				currentPosition = targetPosition[currentTarget];
				currentAngle = targetAngle[currentTarget];
				Game_Complete();
			}
		}
	}
} 

void Game_DetectTargetHit(Point3D planePosition)
{
  // Check if plane is within boundries
  if(planePosition.x > xmin && planePosition.x < xmax 
     && planePosition.z > zmin && planePosition.z < zmax
     && planePosition.y > ymin && planePosition.y < ymax) {
    hitTarget = 1;
  }
}
