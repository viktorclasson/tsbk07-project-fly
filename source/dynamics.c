#include "dynamics.h"

void Dynamics_Init(Point3D* forward, Point3D* up, Point3D* right, 
		   Point3D* position, GLfloat* velocity)
{
  // Set inital airplane coordinat system
  SetVector(0, 1, 0, up);
  SetVector(1, 0, 0, forward);	
  SetVector(0, 0, 1, right);
  
  // Set start position
  SetVector(50, 150, 50, position);
  
  // Set inital velocity
  *velocity = 0;
}

void Dynamics_CalcRot(GLfloat yawRate, GLfloat pitchRate, GLfloat rollRate, 
		 Point3D* forward, Point3D* up, Point3D* right)
{
    GLfloat pitchMat[16], yawMat[16], rollMat[16];
    
    if(pitchRate)
    {
      // Create matrix to rotate around right-vector
      ArbRotate(right, pitchRate, pitchMat);
		
      // Update forward and right matrix
      MatrixMultPoint3D(pitchMat, forward, forward);
      MatrixMultPoint3D(pitchMat, up, up);
    }
    
    if(yawRate)
    {
      // Create matrix to rotate around up-vector
      ArbRotate(up, yawRate, yawMat);
		
      // Update forward and right matrix
      MatrixMultPoint3D(yawMat, forward,forward);
      MatrixMultPoint3D(yawMat, right, right);
    }
    
    if(rollRate)
    {
      // Create matrix to rotate around forward-vector
      ArbRotate(forward, rollRate, rollMat);
		
      // Update forward and right matrix
      MatrixMultPoint3D(rollMat, up, up);
      MatrixMultPoint3D(rollMat, right, right);
    }
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