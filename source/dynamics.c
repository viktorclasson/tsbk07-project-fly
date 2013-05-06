#include "dynamics.h"

GLfloat yawSpeed, pitchSpeed, rollSpeed;
int yawDir, pitchDir, rollDir;

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
}

void Dynamics_CalcRot(GLfloat yawInput, GLfloat pitchInput, GLfloat rollInput, 
		 Point3D* forward, Point3D* up, Point3D* right)
{
    GLfloat pitchMat[16], yawMat[16], rollMat[16];
    
    /* ******* HANDLE PITCH  ******* */

    // If user input, save the sign of the latest input
    if(pitchInput) pitchDir = sign(pitchInput);

    // Update pitch speed
    pitchSpeed = pitchDir*fabs(pAlpha*pitchSpeed + pitchInput);

    // Create matrix to rotate around right-vector
    ArbRotate(right, pitchSpeed, pitchMat);
    
    // Update forward and right matrix
    MatrixMultPoint3D(pitchMat, forward, forward);
    MatrixMultPoint3D(pitchMat, up, up);
    
    /* ******* HANDLE YAW  ******* */

    // If user input, save the sign of the latest input
    if(yawInput) yawDir = sign(yawInput);

    // Update pitch speed
    yawSpeed = yawDir*fabs(pAlpha*yawSpeed + yawInput);

    // Create matrix to rotate around up-vector
    ArbRotate(up, yawSpeed, yawMat);
		
    // Update forward and right matrix
    MatrixMultPoint3D(yawMat, forward,forward);
    MatrixMultPoint3D(yawMat, right, right);
   
    /* ******* HANDLE ROLL  ******* */

    // If user input, save the sign of the latest input
    if(rollInput) rollDir = sign(rollInput);

    // Update pitch speed
    rollSpeed = rollDir*fabs(pAlpha*rollSpeed + rollInput);

    // Create matrix to rotate around forward-vector
    ArbRotate(forward, rollSpeed, rollMat);
		
    // Update forward and right matrix
    MatrixMultPoint3D(rollMat, up, up);
    MatrixMultPoint3D(rollMat, right, right);    
}

void Dynamics_CalcPos(GLfloat thrust, Point3D* forward, GLfloat* velocity, Point3D* position)
{
  	Point3D movment;
	*velocity = thrust; // Utveckla denna!
	// Scale forward vector with velocity
	ScalarMult(forward, *velocity, &movment);
	// Add movment vector to position
	VectorAdd(position, &movment, position);
}
