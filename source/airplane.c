#include "airplane.h"

// Shader matrices
GLfloat projMatrix[16], mdlMatrix[16], normalMatrix[16];

// Models
ExtendedModel **planeModels;
GLuint modelCount;

// Scale factor for airplane model
GLfloat scaleFactor = 0.5;

// Initial rotation for airplane model
GLfloat initialRotY = -3.14159265358979323846/2;

// Textures
GLuint planeTex;

// Shaders
GLuint plane_program;

ExtendedModel* LoadExtendedModel(char *name, Point3D *Kd, Point3D *Ka, Point3D *Ks, GLfloat Ns, GLfloat Tr, char *texture)
{
  ExtendedModel *m = malloc(sizeof(Model));
  
  Model *model;  
  model = LoadModelPlus(name);
  m->model = model;
  m->Kd = *Kd;
  m->Ka = *Ka;
  m->Ks = *Ks;
  m->Ns = Ns;
  m->Tr = Tr;
  
  if(texture != NULL)
  {
    LoadTGATextureSimple(texture, &(m->texture));
    m->hasTexture = 1;
  }
  else
  {
    m->hasTexture = 0;
  }
  
  return m;
}

void DrawExtendedModel(ExtendedModel *m, GLuint program, char* vertexVariableName, char* normalVariableName, char* texCoordVariableName)
{
  if(m->hasTexture == 1)
  {
    // Upload textures
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m->texture);
  }
  
  // Draw the plane
  DrawModel(m->model, program, "inPosition", "inNormal", "inTexCoord");
}

ExtendedModel** Models_Init(void)
{
  modelCount = 6;
  
  ExtendedModel** models = malloc(sizeof(ExtendedModel*) * modelCount);
  
  Point3D Kd, Ka, Ks;
  GLfloat Ns, Tr;
  
  //harrier_g0
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[0] = LoadExtendedModel("objects/harrier/harrier_g0_0.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/ADEN.tga");
  
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[1] = LoadExtendedModel("objects/harrier/harrier_g0_1.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/Camera.tga");
  
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[2] = LoadExtendedModel("objects/harrier/harrier_g0_2.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/SNEB.tga");
  
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[3] = LoadExtendedModel("objects/harrier/harrier_g0_3.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/Tank.tga");
  
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[4] = LoadExtendedModel("objects/harrier/harrier_g0_4.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/Default.tga");
  
  SetVector(0.627451, 0.752941, 0.878431, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[5] = LoadExtendedModel("objects/harrier/harrier_g0_0.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/ADEN.tga");
  
  return models;
}

void Airplane_Init(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView, GLuint* resetFlag)
{
  plane_program = loadShaders("shaders/airplane.vert","shaders/airplane.frag");
  glUseProgram(plane_program);
  
  planeModels = Models_Init();
  
  
  *thrust = 0;
  *yawRate = 0;
  *pitchRate = 0;
  *rollRate = 0;
  *firstPersonView = 0;
  
  *resetFlag = 0;
  
  // Projection
  frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projMatrix);
}

void Airplane_Keyboard(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView, GLuint* resetFlag)
{
  GLfloat pitchModifier = 0.0025;
  GLfloat rollModifier = 0.01;
  GLfloat yawModifier = 0.001;
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
    *thrust = fmax(*thrust - thrustModifier,0);
  }
  
  if(keyIsDown('1')) //set first person view
  {
    *firstPersonView = 1;
  }
  else if(keyIsDown('3')) //set third person view
  {
    *firstPersonView = 0;
  }
  
  if(keyIsDown('r')) //reset game
  {
    *resetFlag = 1;
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
  
  for(int i = 0; i < modelCount; i++)
    DrawExtendedModel(planeModels[i], plane_program, "inPosition", "inNormal", "inTexCoord");
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
  for (i = 0; i < planeModels[4]->model->numVertices; i++)
  {
    if (planeModels[4]->model->vertexArray[3 * i] < *back) *back = planeModels[4]->model->vertexArray[3 * i]; //xmin
    if (planeModels[4]->model->vertexArray[3 * i] > *front) *front = planeModels[4]->model->vertexArray[3 * i]; //xmax
    if (planeModels[4]->model->vertexArray[3 * i+1] < *bottom) *bottom = planeModels[4]->model->vertexArray[3 * i+1]; //ymin
    if (planeModels[4]->model->vertexArray[3 * i+1] > *top) *top = planeModels[4]->model->vertexArray[3 * i+1]; //ymax
    if (planeModels[4]->model->vertexArray[3 * i+2] < *rightWing) *rightWing = planeModels[4]->model->vertexArray[3 * i+2]; //zmin
    if (planeModels[4]->model->vertexArray[3 * i+2] > *leftWing) *leftWing = planeModels[4]->model->vertexArray[3 * i+2]; //zmax
  }

  // Scale according to scale factor
  *bottom = *bottom * scaleFactor;
  *top = *top * scaleFactor;
  *back = *back * scaleFactor;
  *front = *front * scaleFactor;
  *leftWing = *leftWing * scaleFactor;
  *rightWing = *rightWing * scaleFactor;

}
	
