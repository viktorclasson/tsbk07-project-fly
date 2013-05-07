#include "dynamics.h"

// Dyanamic of airplane
GLfloat yawSpeed, pitchSpeed, rollSpeed;
int yawDir, pitchDir, rollDir;
GLfloat yaw, pitch, roll;

// Varibles to handle falling
GLfloat fallSpeed;

// State machine
int pitchState, rollState;

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

  // Inititate fall speed
  fallSpeed = 0.01;

  // Inititate state machine
  pitchState = 0, rollState;
}

void Dynamics_CalcRot(GLfloat yawInput, GLfloat pitchInput, GLfloat rollInput, 
		 Point3D* forward, Point3D* up, Point3D* right)
{
    GLfloat pitchMat[16], yawMat[16], rollMat[16];
    
    /* ----- STATE MACHINE -----
       0 = normal: no pitch, no pitch velocity
       1 = user input:
       2 = delay: no user input
       3 = pitch back: enter when pitch velocity is small, exit when pitch is small
       4 = correcting: enter pitch is small, exit when angle is zero
    */

    /* --------------- HANDLE PITCH --------------- */

    // Update state
    if(pitchInput) {
      pitchState = 1;
      pitchDir = sign(pitchInput);
    }
    else if(pitchState == 1 && !pitchInput) {
      pitchState = 2;
    }
    else if(pitchState == 2 && fabs(pitchSpeed) < AngVelEpsilon && fabs(pitch) < pitchBackLimit) {
      pitchState = 3;
    }
    else if(pitchState == 3 && fabs(pitch) < AngleEpsilon) {
      pitchState = 4;
    }
    else if(pitchState == 4 && !pitch) {
      pitchState = 0;
    }

    // Update pitch speed depning on state
    switch (pitchState) {
    case 0:
      pitchSpeed = 0;
      break;
    case 1:
      pitchSpeed = pitchDir*fabs(pitchAlpha*pitchSpeed + pitchInput);
      break;
    case 2:
      pitchSpeed = pitchDir*fabs(pitchAlpha*pitchSpeed);
      break;
    case 3:
      pitchSpeed = -sign(pitch)*fabs(pitchAlpha*pitchSpeed + pitch*pitchBackModifier);
      break;
    case 4:
      pitchSpeed = -pitch;
      break;
    }

    // Accumulate pitch
    pitch += pitchSpeed;

    // Create matrix to rotate around right-vector
    ArbRotate(right, pitchSpeed, pitchMat);
    
    // Update forward and right matrix
    MatrixMultPoint3D(pitchMat, forward, forward);
    MatrixMultPoint3D(pitchMat, up, up);


    /* ******* HANDLE YAW  ******* */

    // If user input, save the sign of the latest input
    if(yawInput) 
      yawDir = sign(yawInput);

    // Update pitch speed
    if(yawSpeed > AngVelEpsilon || yawInput)
      yawSpeed = yawDir*fabs(yawAlpha*yawSpeed + yawInput);
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

    if(rollInput) {
      rollState = 1;
      rollDir = sign(rollInput);
    }
    else if(rollState == 1 && !rollInput) {
      rollState = 2;
    }
    else if(rollState == 2 && fabs(rollSpeed) < AngVelEpsilon && fabs(roll) < rollBackLimit) {
      rollState = 3;
    }
    else if(rollState == 3 && fabs(roll) < AngleEpsilon) {
      rollState = 4;
    }
    else if(rollState == 4 && !roll) {
      rollState = 0;
      rollSpeed = 0;
    }

    switch (rollState) {
    case 0:
      break;
    case 1:
      rollSpeed = rollDir*fabs(rollAlpha*rollSpeed + rollInput);
      break;
    case 2:
      rollSpeed = rollDir*fabs(rollAlpha*rollSpeed);
      break;
    case 3:
      rollSpeed = -sign(roll)*fabs(rollAlpha*rollSpeed + roll*rollBackModifier);
      break;
    case 4:
      rollSpeed = -roll;
      break;
    }

    // Accumulate roll angle
    roll += rollSpeed;

    // Create matrix to rotate around forward-vector
    ArbRotate(forward, rollSpeed, rollMat);
		
    // Update forward and right matrix
    MatrixMultPoint3D(rollMat, up, up);
    MatrixMultPoint3D(rollMat, right, right);

    /*
    printf("Pitch: %f, Pitch rate: %f, Pitch input: %f, Pitch state: %d\n", 
	   pitch, pitchSpeed, pitchInput, pitchState);
    printf("Roll: %f, Roll rate: %f, Roll input: %f, Roll state: %d\n", 
	   roll, rollSpeed, rollInput, rollState);
    */
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

