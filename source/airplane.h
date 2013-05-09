#ifndef AIRPLANE
#define AIRPLANE

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include "utils/VectorUtils2.h"
#include "utils/GL_utilities.h"
#include "utils/loadobj.h"
#include "utils/LoadTGA2.h"

#include <stdlib.h>
#include <string.h>

typedef struct
{
  Model* model;
  Point3D Kd;
  Point3D Ka;
  Point3D Ks;
  GLfloat Ns;
  GLfloat Tr;
  GLuint hasTexture;
  GLuint texture;
} ExtendedModel;

ExtendedModel* LoadExtendedModel(char *name, Point3D *Kd, Point3D *Ka, Point3D *Ks, GLfloat Ns, GLfloat Tr, char *texture);
void DrawExtendedModel(ExtendedModel *m, Point3D* camPosition, GLuint untexturedProgram, GLuint texturedProgram, char* vertexVariableName, char* normalVariableName, char* texCoordVariableName);

void Airplane_Init(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView, GLuint* resetFlag);
void Airplane_Keyboard(GLfloat* thrust, GLfloat* yawRate, GLfloat* pitchRate, GLfloat* rollRate, GLuint* firstPersonView, GLuint* resetFlag);
void Airplane_Draw(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, Point3D* camPosition, GLfloat* camMatrix);
void Airplane_CalcMatrices(Point3D* forward, Point3D* up, Point3D* right, Point3D* position, GLfloat* camMatrix, GLfloat* mdlMatrix, GLfloat* normalMatrix);
void Airplane_FindEdges(GLfloat* front, GLfloat* back, GLfloat* leftWing, GLfloat* rightWing, GLfloat* top, GLfloat* bottom);

#endif
