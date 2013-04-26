#include "camera.h"

void Camera_Init(Point3D* forward, Point3D* up, Point3D* position, 
		 GLfloat velocity, Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix)
{
  Camera_Update(forward, up, position, velocity, camera_position, camera_look, camMatrix);
}


void Camera_Update(Point3D* forward, Point3D* up, Point3D* position, 
		 GLfloat velocity, Point3D* camera_position, Point3D* camera_look, GLfloat* camMatrix)
{
  Point3D backward, camPosition, lookPosition;
  
  // Calculating backwards vector
  ScalarMult(forward, -1, &backward);
  
  // Calculating camera position (behind and above(in plane coordinates) the plane)
  ScalarMult(&backward, 15, &backward);
  ScalarMult(up, 5, &camPosition);
  VectorAdd(&camPosition, &backward, &camPosition);
  VectorAdd(&camPosition, position, &camPosition);
  
  // Calculating look at position (slightly above(in world coordinates) the plane)
  ScalarMult(up, 3, &lookPosition);
  VectorAdd(&lookPosition, position, &lookPosition);
  
  // Calculating camera matrix from camera and look at positions
  lookAt(&camPosition, &lookPosition, up->x, up->y, up->z, camMatrix);
  
  
  // Updating camera position and look global vectors(for use in world)
  CopyVector(&camPosition, camera_position);
  CopyVector(&lookPosition, camera_look);
}