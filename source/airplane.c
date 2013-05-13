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

// Shaders
GLuint plane_textured_program;
GLuint plane_untextured_program;

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

void DrawExtendedModel(ExtendedModel *m, Point3D* camPosition, GLuint untexturedProgram, GLuint texturedProgram, char* vertexVariableName, char* normalVariableName, char* texCoordVariableName)
{
  // Light source data
  GLfloat ambientLightLevel[3] = { 1.0, 1.0, 1.0 };
  GLfloat sourceLightLevel[3] = { 1.0, 1.0, 1.0 };
  GLfloat sourceDirection[3] = { -0.654951, 0.680844, 0.327942  };
  
  // Prepare material data for upload
  GLfloat Ka[3] = { m->Ka.x, m->Ka.y, m->Ka.z };
  GLfloat Kd[3] = { m->Kd.x, m->Kd.y, m->Kd.z };
  GLfloat Ks[3] = { m->Ks.x, m->Ks.y, m->Ks.z };
  
  // Prepare camera position for upload
  GLfloat camPos[3] = { camPosition->x, camPosition->y, camPosition->z };
  
  if(m->hasTexture == 1)
  {
    glUseProgram(texturedProgram);
    
    // Upload textures
    glUniform1i(glGetUniformLocation(texturedProgram, "texUnit"), 0); // Texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m->texture);
    
    // Upload light, material and camera info
    glUniform3fv(glGetUniformLocation(plane_textured_program, "ambientLightLevel"), 1, ambientLightLevel);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "sourceLightLevel"), 1, sourceLightLevel);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "sourceDirection"), 1, sourceDirection);
    
    glUniform3fv(glGetUniformLocation(plane_textured_program, "Ka"), 1, Ka);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "Kd"), 1, Kd);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "Ks"), 1, Ks);
    glUniform1f(glGetUniformLocation(plane_textured_program, "Ns"), m->Ns);
    glUniform1f(glGetUniformLocation(plane_textured_program, "Tr"), m->Tr);
    
    glUniform3fv(glGetUniformLocation(plane_textured_program, "camPosition"), 1, camPos);
    
    // Draw the plane
    DrawModel(m->model, texturedProgram, vertexVariableName, normalVariableName, texCoordVariableName);
  }
  else
  {
    glUseProgram(plane_untextured_program);
    
    // Upload light and material info
    glUniform3fv(glGetUniformLocation(plane_textured_program, "ambientLightLevel"), 1, ambientLightLevel);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "sourceLightLevel"), 1, sourceLightLevel);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "sourceDirection"), 1, sourceDirection);
    
    glUniform3fv(glGetUniformLocation(plane_textured_program, "Ka"), 1, Ka);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "Kd"), 1, Kd);
    glUniform3fv(glGetUniformLocation(plane_textured_program, "Ks"), 1, Ks);
    glUniform1f(glGetUniformLocation(plane_textured_program, "Ns"), m->Ns);
    glUniform1f(glGetUniformLocation(plane_textured_program, "Tr"), m->Tr);
    
    // Draw the plane
    DrawModel(m->model, untexturedProgram, vertexVariableName, normalVariableName, texCoordVariableName);
  }
}

ExtendedModel** Models_Init(void)
{
  modelCount = 26;
  
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
  models[4] = LoadExtendedModel("objects/harrier/harrier_g0_4.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/USMC.tga");
  
  SetVector(0.627451, 0.752941, 0.878431, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[5] = LoadExtendedModel("objects/harrier/harrier_g0_5.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g1_Knobs
  SetVector(0.627451, 0.752941, 0.878431, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[6] = LoadExtendedModel("objects/harrier/harrier_g1_Knobs_5.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[7] = LoadExtendedModel("objects/harrier/harrier_g1_Knobs_6.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(0.233333, 0.233333, 0.233333, &Kd);
  SetVector(0, 0, 0, &Ka);
  SetVector(0.9, 0.9, 0.9, &Ks);
  Ns = 32;
  Tr = 0;
  models[8] = LoadExtendedModel("objects/harrier/harrier_g1_Knobs_7.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(0.8, 0.8, 0.8, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[9] = LoadExtendedModel("objects/harrier/harrier_g1_Knobs_8.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g2
  SetVector(0.627451, 0.752941, 0.878431, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[10] = LoadExtendedModel("objects/harrier/harrier_g2_5.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[11] = LoadExtendedModel("objects/harrier/harrier_g2_6.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(0.233333, 0.233333, 0.233333, &Kd);
  SetVector(0, 0, 0, &Ka);
  SetVector(0.9, 0.9, 0.9, &Ks);
  Ns = 32;
  Tr = 0;
  models[12] = LoadExtendedModel("objects/harrier/harrier_g2_7.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g2_ThrottleLever
  SetVector(0.233333, 0.233333, 0.233333, &Kd);
  SetVector(0, 0, 0, &Ka);
  SetVector(0.9, 0.9, 0.9, &Ks);
  Ns = 32;
  Tr = 0;
  models[13] = LoadExtendedModel("objects/harrier/harrier_g2_ThrottleLever.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g3_VectorLevel
  SetVector(0.233333, 0.233333, 0.233333, &Kd);
  SetVector(0, 0, 0, &Ka);
  SetVector(0.9, 0.9, 0.9, &Ks);
  Ns = 32;
  Tr = 0;
  models[14] = LoadExtendedModel("objects/harrier/harrier_g3_VectorLevel.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g4
  SetVector(0.627451, 0.752941, 0.878431, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[15] = LoadExtendedModel("objects/harrier/harrier_g4.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g4_Stick
  SetVector(0.233333, 0.233333, 0.233333, &Kd);
  SetVector(0, 0, 0, &Ka);
  SetVector(0.9, 0.9, 0.9, &Ks);
  Ns = 32;
  Tr = 0;
  models[16] = LoadExtendedModel("objects/harrier/harrier_g4_Stick_7.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(1.0, 0.0, 0.0, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[17] = LoadExtendedModel("objects/harrier/harrier_g4_Stick_9.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g5_LPedal
  SetVector(0.533333, 0.533333, 0.533333, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[18] = LoadExtendedModel("objects/harrier/harrier_g5_LPedal.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g5_RPedal
  SetVector(0.533333, 0.533333, 0.533333, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[19] = LoadExtendedModel("objects/harrier/harrier_g5_RPedal.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g7
  SetVector(0.233333, 0.233333, 0.233333, &Kd);
  SetVector(0, 0, 0, &Ka);
  SetVector(0.9, 0.9, 0.9, &Ks);
  Ns = 32;
  Tr = 0;
  models[20] = LoadExtendedModel("objects/harrier/harrier_g7_7.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(0.67, 0.88, 0.68, &Kd);
  SetVector(0.67, 0.88, 0.68, &Ka);
  SetVector(1.0, 1.0, 1.0, &Ks);
  Ns = 128;
  Tr = 0.8;
  models[21] = LoadExtendedModel("objects/harrier/harrier_g7_11.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(0.454902, 0.521569, 0.454902, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0;
  models[22] = LoadExtendedModel("objects/harrier/harrier_g7_12.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  //harrier_g7_group
  /*
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[23] = LoadExtendedModel("objects/harrier/harrier_g7_group_13.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/reflection.tga");
  */
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[23] = LoadExtendedModel("objects/harrier/harrier_g7_group_13.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  SetVector(1.0, 1.0, 1.0, &Kd);
  SetVector(1.0, 1.0, 1.0, &Ka);
  SetVector(0.5, 0.5, 0.5, &Ks);
  Ns = 64;
  Tr = 0;
  models[24] = LoadExtendedModel("objects/harrier/harrier_g7_group_14.obj", &Kd, &Ka, &Ks, Ns, Tr, "textures/harrier/Pilot.tga");
  
  //harrier_g8
  SetVector(0.8, 0.8, 0.8, &Kd);
  SetVector(0.2, 0.2, 0.2, &Ka);
  SetVector(0.2, 0.2, 0.2, &Ks);
  Ns = 128;
  Tr = 0.902;
  models[25] = LoadExtendedModel("objects/harrier/harrier_g8.obj", &Kd, &Ka, &Ks, Ns, Tr, NULL);
  
  return models;
}

void Airplane_Init(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView, GLuint* resetFlag)
{
  plane_textured_program = loadShaders("shaders/airplane_textured.vert","shaders/airplane_textured.frag");
  plane_untextured_program = loadShaders("shaders/airplane_untextured.vert","shaders/airplane_untextured.frag");
  
  planeModels = Models_Init();
  
  
  *thrust = 3;
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


void Airplane_Draw(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, Point3D* camPosition, GLfloat* camMatrix)
{
  // Calculate complete model matrix and normal matrix(camera and projection applied here, no need to calculate for every vertex on GPU)
  Airplane_CalcMatrices(forward, up, right, position, camMatrix, mdlMatrix, normalMatrix);
  
  // Upload model and normal matrices to both programs
  glUseProgram(plane_textured_program);
  glUniformMatrix4fv(glGetUniformLocation(plane_textured_program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix);
  glUniformMatrix3fv(glGetUniformLocation(plane_textured_program, "normalMatrix"), 1, GL_TRUE, normalMatrix);
  
  glUseProgram(plane_untextured_program);
  glUniformMatrix4fv(glGetUniformLocation(plane_untextured_program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix);
  glUniformMatrix3fv(glGetUniformLocation(plane_untextured_program, "normalMatrix"), 1, GL_TRUE, normalMatrix);
  
  // Draw all models
  for(int i = 0; i < modelCount; i++)
    DrawExtendedModel(planeModels[i], camPosition, plane_untextured_program, plane_textured_program, "inPosition", "inNormal", "inTexCoord");
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
	
