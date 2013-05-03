#include "airplane.h"

// Shader matrices
GLfloat projMatrix[16], mdlMatrix[16], normalMatrix[16];

// Models
Model *plane;

// Scale factor for airplane model
GLfloat scaleFactor = 0.5;

// Initial rotation for airplane model
GLfloat initialRotY = -3.14159265358979323846/2;

// Textures
GLuint planeTex;

// Shaders
GLuint plane_program;

void Airplane_Init(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView)
{
  plane = LoadModelPlus("objects/Harrier-GR.1.obj");
  
  plane_program = loadShaders("shaders/airplane.vert","shaders/airplane.frag");
  
  glUseProgram(plane_program);
  LoadTGATextureSimple("textures/harrier.tga", &planeTex);
  
  
  *thrust = 0;
  *yawRate = 0;
  *pitchRate = 0;
  *rollRate = 0;
  *firstPersonView = 0;
  
  // Projection
  frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projMatrix);
}

void Airplane_Keyboard(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView)
{
  GLfloat pitchModifier = 0.04;
  GLfloat rollModifier = 0.04;
  GLfloat yawModifier = 0.01;
  GLfloat thrustModifier = 0.05;
  
  if(keyIsDown('w')) // pitch up
  {
    *pitchRate = pitchModifier;
  }
  else if (keyIsDown('s')) //pitch down
  {
    *pitchRate = -1 * pitchModifier;
  }
  else
  {
    *pitchRate = 0;
  }
  
  if (keyIsDown('a')) //roll left
  {
    *rollRate = -1 * rollModifier;
  }
  else if(keyIsDown('d')) //roll right
  {
    *rollRate = rollModifier;
  }
  else
  {
    *rollRate = 0;
  }
  if (keyIsDown('q')) //bank left
  {
    *yawRate = yawModifier;
  }
  else if(keyIsDown('e')) //bank right
  {
    *yawRate = -1 * yawModifier;
  }
  else
  {
    *yawRate = 0;
  }
  
  if (keyIsDown(' ')) //increase thrust
  {
    *thrust = *thrust + thrustModifier;
  }
  else if(keyIsDown('v')) //decrease thust
  {
    *thrust = *thrust - thrustModifier;
  }
  
  if(keyIsDown('1')) //set first person view
  {
    *firstPersonView = 1;
  }
  else if(keyIsDown('3')) //set third person view
  {
    *firstPersonView = 0;
  }
  
  if(keyIsDown('r')) //decrease thust
  {
    *pitchRate = 0;
    *rollRate = 0;
    *yawRate = 0;
    *thrust = 0;
  }
}


void Airplane_Draw(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* camMatrix)
{
  // Calculate complete model matrix and normal matrix(camera and projection applied here, no need to calculate for every vertex on GPU)
  Airplane_CalcMatrices(forward, up, right, position, camMatrix, mdlMatrix, normalMatrix);
  
  glUseProgram(plane_program);
  
  // Upload model and normal matrices
  glUniformMatrix4fv(glGetUniformLocation(plane_program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix);
  glUniformMatrix3fv(glGetUniformLocation(plane_program, "normalMatrix"), 1, GL_TRUE, normalMatrix);
  
  // Upload textures
  glUniform1i(glGetUniformLocation(plane_program, "texUnit"), 0); // Texture unit 1
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, planeTex);
  
  // Draw the plane
  DrawModel(plane, plane_program, "inPosition", "inNormal", "inTexCoord");
  
}

void Airplane_CalcMatrices(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* camMatrix, GLfloat* mdlMatrix, GLfloat* normalMatrix)
{
  GLfloat rotMatrix[16], transMatrix[16], scaleMatrix[16], initialRotMatrix[16];
  
  // Make a scaling matrix
  S(scaleFactor, scaleFactor, scaleFactor, scaleMatrix);
  
  // Create initial rotation matrix
  Ry(initialRotY, initialRotMatrix);
  
  // Put airplane base vectors into rotation matrix, COLUMN WISE!!!
  rotMatrix[0] = right->x ; rotMatrix[1] = up->x; rotMatrix[2] = forward->x; rotMatrix[3] = 0;
  rotMatrix[4] = right->y ; rotMatrix[5] = up->y; rotMatrix[6] = forward->y; rotMatrix[7] = 0;
  rotMatrix[8] = right->z ; rotMatrix[9] = up->z; rotMatrix[10] = forward->z; rotMatrix[11] = 0;
  rotMatrix[12] = 0 ; rotMatrix[13] = 0; rotMatrix[14] = 0; rotMatrix[15] = 1;
  
  // Make a translation matrix
  T(position->x, position->y, position->z, transMatrix);
 /*
  // Put them together to the model matrix, and apply model-to-view matrix
  Mult(initialRotMatrix, scaleMatrix, mdlMatrix); 
  Mult(rotMatrix, mdlMatrix, mdlMatrix); 
  Mult(transMatrix, mdlMatrix, mdlMatrix);
  Mult(camMatrix, mdlMatrix, mdlMatrix);
  */
   // Put them together to the model matrix, and apply model-to-view matrix
  Mult(rotMatrix, initialRotMatrix, mdlMatrix); 
  Mult(scaleMatrix, mdlMatrix, mdlMatrix); 
  Mult(transMatrix, mdlMatrix, mdlMatrix);
  Mult(camMatrix, mdlMatrix, mdlMatrix);
  
  // Extract the normal matrix from the model matrix
  normalMatrix[0] = mdlMatrix[0]; normalMatrix[1] = mdlMatrix[1]; normalMatrix[2] = mdlMatrix[2];
  normalMatrix[4] = mdlMatrix[4]; normalMatrix[5] = mdlMatrix[5]; normalMatrix[6] = mdlMatrix[6]; 
  normalMatrix[8] = mdlMatrix[8]; normalMatrix[9] = mdlMatrix[9]; normalMatrix[10] = mdlMatrix[10]; 
  
  // Complete the model matrix by applying projection  
  Mult(projMatrix, mdlMatrix, mdlMatrix); 
}

void Airplane_FindEdges(GLfloat* front, GLfloat* back, GLfloat* leftWing, GLfloat* rightWing, GLfloat* top, GLfloat* bottom)
{
  int i;
  *bottom = 1e10;
  *top = 1e-10;
  *back = 1e10;
  *front = 1e-10;
  *leftWing = 1e-10;
  *rightWing = 1e10;
  for (i = 0; i < plane->numVertices; i++)
  {
    if (plane->vertexArray[3 * i] < *back) *back = plane->vertexArray[3 * i]; //xmin
    if (plane->vertexArray[3 * i] > *front) *front = plane->vertexArray[3 * i]; //xmax
    if (plane->vertexArray[3 * i+1] < *bottom) *bottom = plane->vertexArray[3 * i+1]; //ymin
    if (plane->vertexArray[3 * i+1] > *top) *top = plane->vertexArray[3 * i+1]; //ymax
    if (plane->vertexArray[3 * i+2] < *rightWing) *rightWing = plane->vertexArray[3 * i+2]; //zmin
    if (plane->vertexArray[3 * i+2] > *leftWing) *leftWing = plane->vertexArray[3 * i+2]; //zmax
  }

  // Scale according to scale factor
  *bottom = *bottom * scaleFactor;
  *top = *top * scaleFactor;
  *back = *back * scaleFactor;
  *front = *front * scaleFactor;
  *leftWing = *leftWing * scaleFactor;
  *rightWing = *rightWing * scaleFactor;

}
	
