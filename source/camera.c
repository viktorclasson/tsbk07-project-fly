#include "camera.h"

void Camera_Init(GLuint firstPersonView, Point3D* forward, Point3D* up, Point3D* position, 
		 GLfloat velocity, Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix)
{
  Camera_Update(firstPersonView, forward, up, position, velocity, camera_position, camera_look, camMatrix);
}


void Camera_Update(GLuint firstPersonView, Point3D* forward, Point3D* up, Point3D* position, 
		 GLfloat velocity, Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix)
{
  Point3D backward, camPosition, lookPosition, tempPoint3D;
  
  if(firstPersonView == 0) // Calculate camera matrix for third person view
  {
    // Calculating backwards vector
    ScalarMult(forward, -1, &backward);
/*    
    // Calculating camera position (behind and above(in plane coordinates) the plane)
    ScalarMult(&backward, 15, &backward);
    ScalarMult(up, 5, &camPosition);
    VectorAdd(&camPosition, &backward, &camPosition);
    VectorAdd(&camPosition, position, &camPosition);
    
    // Calculating look at position (slightly above(in world coordinates) the plane)
    ScalarMult(up, 3, &lookPosition);
    VectorAdd(&lookPosition, position, &lookPosition);
*/    

    // Calculating camera position (behind and above(in plane coordinates) the plane)
    SetVector(backward.x, 0, backward.z, &tempPoint3D);
    Normalize(&tempPoint3D);
    ScalarMult(&tempPoint3D, 15, &camPosition);
    //ScalarMult(up, 0, &camPosition);
    //VectorAdd(&camPosition, &tempPoint3D, &camPosition);
    VectorAdd(&camPosition, position, &camPosition);
    
    // Calculating look at position (slightly above(in world coordinates) the plane)
    //ScalarMult(up, 0, &lookPosition);
    //VectorAdd(&lookPosition, position, &lookPosition);
    CopyVector(position, &lookPosition);

    // Calculating camera matrix from camera and look at positions
    lookAt(&camPosition, &lookPosition, 0, 1, 0, camMatrix);
    
    
    // Updating camera position and look global vectors(for use in world)
    CopyVector(&camPosition, camera_position);
    CopyVector(&lookPosition, camera_look);
  }
  else
  {     
    // Calculating camera position (slightly in front of and above plane origo)
    ScalarMult(forward, 1.3, &camPosition);
    ScalarMult(up, 0.1, &tempPoint3D);
    VectorAdd(&camPosition, &tempPoint3D, &camPosition);
    VectorAdd(&camPosition, position, &camPosition);
    
    // Calculating look at position (in front of the plane)
    ScalarMult(forward, 3, &lookPosition);
    VectorAdd(&lookPosition, &tempPoint3D, &lookPosition);
    VectorAdd(&lookPosition, position, &lookPosition);
    
    // Calculating camera matrix from camera and look at positions
    lookAt(&camPosition, &lookPosition, up->x, up->y, up->z, camMatrix);
    
    
    // Updating camera position and look global vectors(for use in world)
    CopyVector(&camPosition, camera_position);
    CopyVector(&lookPosition, camera_look);
  }
}