#ifndef DYNAMICS
#define DYNAMICS

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/gl.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils/VectorUtils2.h"

#define steadyState 0
#define userInput 1
#define smoothTail 2
#define autoBack 3
#define correction 4
#define pitchAlpha 0.90 // Higher value gives slower fade out
#define pitchBackLimit M_PI/10 // Limit for when airplane should pitch back autonomously
#define pitchBackModifier 0.025 // Higher value gives faster pitch back
#define rollAlpha 0.90 // Higer values gives slower fader out
#define rollBackLimit M_PI/10 // Limit for when airplane should pitch back autonomously
#define rollBackModifier 0.025 // Higher value gives faster pitch back
#define yawAlpha 0.90
#define AngVelEpsilon 0.001 // Just a small angle velocity value
#define AngleEpsilon 0.005 // Just a small angle value
#define maxVelocity 30 // Max translation velocity for airplane
#define thrustRespons 20 // Lower value gives a steeper velocity respons on thrust for low thrust values
#define liftLimit 5 // Lower limit on velocity, flying slower will make the plane fall
#define fallSpeedModifier 20// Lower value gives faster accleration of fall speed

void Dynamics_Init(Point3D* forward, Point3D* up, Point3D* right, 
		   Point3D* position, GLfloat* velocity);

void Dynamics_CalcRot(GLfloat yawInput, GLfloat pitchInput, GLfloat rollInput, 
		 Point3D* forward, Point3D* up, Point3D* right);
		 
void Dynamics_CalcPos(GLfloat thrust, Point3D* forward, GLfloat* velocity, Point3D* position);

#endif
