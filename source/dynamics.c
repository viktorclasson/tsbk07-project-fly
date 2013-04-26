#include "dynamics.h"

void Dynamics_Init(Point3D* forward, Point3D* up, Point3D* right, 
		   Point3D* position, GLfloat* velocity)
{
  // Set inital airplane coordinat system
  SetVector(0, 1, 0, up);
  SetVector(0, 0, 1, forward);	
  SetVector(1, 0, 0, right);
  
  // Set start position
  SetVector(50, 50, 50, position);
  
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

void Dynamics_CalcMdlMatrix(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* mdlMatrix)
{
  GLfloat Rot[16], Trans[16];
  
  // Put airplane base vectors into rotation matrix, COLUMN WISE!!!
  Rot[0] = right->x ; Rot[1] = up->x; Rot[2] = forward->x; Rot[3] = 0;
  Rot[4] = right->y ; Rot[5] = up->y; Rot[6] = forward->y; Rot[7] = 0;
  Rot[8] = right->z ; Rot[9] = up->z; Rot[10] = forward->z; Rot[11] = 0;
  Rot[12] = 0 ; Rot[13] = 0; Rot[14] = 0; Rot[15] = 1;
  
  // Make a translation matrix
  T(position->x, position->y, position->z, Trans);
  
  // Put them together
  Mult(Rot, Trans, mdlMatrix); 
}