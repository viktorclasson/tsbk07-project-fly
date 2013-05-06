#include "dynamics.h"

// Dyanamic of airplane
GLfloat yawSpeed, pitchSpeed, rollSpeed;
int yawDir, pitchDir, rollDir;
GLfloat yaw, pitch, roll;

// Varibles to handle falling
GLfloat fallSpeed;

// Variables to handle automatic pitch/roll back
int pitchBack, rollBack;
GLfloat pitchArtif, rollArtif;

int sign(GLfloat x)
{
  if(x > 0) return 1;
  else if (x < 0) return -1;
  else return 0;
}

void Dynamics_Init(Point3D* forward, Point3D* up, Point3D* right, 
		   Point3D* position, GLfloat* velocity)
{
  // Set inital airplane coordinat system
  SetVector(0, 1, 0, up);
  SetVector(0, 0, 1, forward);	
  SetVector(1, 0, 0, right);
  
  // Set start position
  SetVector(50, 150, 50, position);
  
  // Set inital velocity
  *velocity = 0;

  // Set inital angle velocities
  yawSpeed = 0; pitchSpeed = 0; rollSpeed = 0;

  // Set flags for pitch and roll back
  pitchBack = 0; rollBack = 0;

  fallSpeed = 0.01;
}

void Dynamics_CalcRot(GLfloat yawInput, GLfloat pitchInput, GLfloat rollInput, 
		 Point3D* forward, Point3D* up, Point3D* right)
{
    GLfloat pitchMat[16], yawMat[16], rollMat[16];
    
    /* ******* HANDLE PITCH  ******* */       
    // User input, set sign of input and cancel pitch back 
    if(pitchInput) {
	pitchDir = sign(pitchInput);
	pitchBack = 0;
      }
    // Pitch angle is not small and no pitch rate, start pitch back 
    else if(fabs(pitch) > 0.01 && pitchSpeed == 0)
      {
	pitchBack = 1;
      }
    // Pitch angle is small, stop pitch back
    else if(fabs(pitch) < 0.01) {
      pitchBack = 0;
    }
    
    // Dynamic pitch speed
    if(fabs(pitchSpeed) > 0.001 || pitchInput || pitchBack) {
      // Set pitch speed after pitch back
      if(pitchBack) {
	pitchSpeed = -sign(pitch)*fabs(pAlpha*pitchSpeed - pitch*0.005*pitchBack);
      }
      // Set pitch speed after user input (user input could be zero)
      else {
	pitchSpeed = pitchDir*fabs(pAlpha*pitchSpeed + pitchInput);
      }
    }
    else {
      pitchSpeed = 0;
      pitchBack = 0;
    }

    // Accumulate pitch angle
    pitch += pitchSpeed;

    // Create matrix to rotate around right-vector
    ArbRotate(right, pitchSpeed, pitchMat);
    
    // Update forward and right matrix
    MatrixMultPoint3D(pitchMat, forward, forward);
    MatrixMultPoint3D(pitchMat, up, up);
    
    printf("Pitch: %f, Pitch back: %d, Pitch rate: %f, Pitch input: %f, Artifical input: %f\n", 
	   pitch, pitchBack, pitchSpeed, pitchInput, pitchArtif);

    /* ******* HANDLE YAW  ******* */

    // If user input, save the sign of the latest input
    if(yawInput) 
      yawDir = sign(yawInput);

    // Update pitch speed
    if(yawSpeed > 0.001 || yawInput)
      yawSpeed = yawDir*fabs(pAlpha*yawSpeed + yawInput);
    else
      yawSpeed = 0;

    // Create matrix to rotate around up-vector
    ArbRotate(up, yawSpeed, yawMat);
		
    // Update forward and right matrix
    MatrixMultPoint3D(yawMat, forward,forward);
    MatrixMultPoint3D(yawMat, right, right);
   
    // Accumulate yaw angle
    yaw += yawSpeed;

    /* ******* HANDLE ROLL  ******* */

    // User input, set sign of input and cancel roll back 
    if(rollInput) {
	rollDir = sign(rollInput);
	rollBack = 0;
      }
    // Roll angle is not small and no roll rate, start roll back 
    else if(fabs(roll) > 0.01 && rollSpeed == 0)
      {
	rollBack = 1;
      }
    // Roll angle is small, stop roll back
    else if(fabs(roll) < 0.01) {
      rollBack = 0;
    }
    
    // Dynamic roll speed
    if(fabs(rollSpeed) > 0.001 || rollInput || rollBack) {
      // Set roll speed after roll back
      if(rollBack) {
	rollSpeed = -sign(roll)*fabs(pAlpha*rollSpeed - roll*0.005*rollBack);
      }
      // Set roll speed after user input (user input could be zero)
      else {
	rollSpeed = rollDir*fabs(pAlpha*rollSpeed + rollInput);
      }
    }
    else {
      rollSpeed = 0;
      rollBack = 0;
    }

    // Accumulate roll angle
    roll += rollSpeed;

    // Create matrix to rotate around forward-vector
    ArbRotate(forward, rollSpeed, rollMat);
		
    // Update forward and right matrix
    MatrixMultPoint3D(rollMat, up, up);
    MatrixMultPoint3D(rollMat, right, right);    
}

void Dynamics_CalcPos(GLfloat thrust, Point3D* forward, GLfloat* velocity, Point3D* position)
{
  	Point3D movment;
	*velocity = maxVelocity*thrust/(thrust + thrustRespons); // Utveckla denna!
	// Scale forward vector with velocity
	ScalarMult(forward, *velocity, &movment);
	// Add movment vector to position
	VectorAdd(position, &movment, position);

	// Check lift
	if (*velocity < 2.5)
	  {
	    position->y -= fallSpeed;
	    fallSpeed += 0.05*fallSpeed;
	  }
}

