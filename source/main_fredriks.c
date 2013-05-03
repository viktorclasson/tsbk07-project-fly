// I believe I can fly, a flight simulator

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif

#include "utils/GL_utilities.h"
#include "utils/VectorUtils2.h"
#include "utils/loadobj.h"
#include "utils/LoadTGA2.h"

#include <math.h>

// Shader matrices
GLfloat projMatrix[16], mdlMatrix[16], normalMatrix[16], camMatrix[16];

// vertex array object
Model *plane;

// Reference to shader program
GLuint plane_program;


void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(1,1,1,0);;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	printError("GL inits");
	
	
	plane = LoadModelPlus("objects/Harrier-GR.1.obj");
	
	plane_program = loadShaders("shaders/airplane.vert","shaders/airplane.frag");
	
	glUseProgram(plane_program);
	LoadTGATextureSimple("textures/harrier.tga", &planeTex);
	
	
	// Projection
	frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 200.0, projMatrix);
	

}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int x_click;
int y_click;

void mousemove(int x, int y)
{
	Point3D position_to_look;
	Point3D look_up;
	GLfloat phi_scale = 100;
	GLfloat theta_scale = 30;
	GLfloat rot_matrix[16];
	
	// Update x and z in camera_look (look around)
	VectorSub(&camera_look, &camera_position, &position_to_look);
	Ry((GLfloat)(x-x_click)/phi_scale, rot_matrix);
	MatrixMultPoint3D(rot_matrix, &position_to_look, &position_to_look);
	VectorAdd(&camera_position, &position_to_look, &camera_look);
	
	// Update y in camera_look (look up and down)
	look_up.x = 0; look_up.z = 0; look_up.y = (y-y_click)/theta_scale;
	VectorAdd(&camera_look, &look_up, &camera_look);
	
	x_click = x;
	y_click = y;
}

void mouseclick(int button, int state, int x, int y)
{
	x_click = x;
	y_click = y;
}

void display(void)
{

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (800, 800);
	glutCreateWindow ("I believe I can fly");
	glutDisplayFunc(display);
	init ();
	initKeymapManager();
	glutTimerFunc(20, &timer, 0);
	glutMouseFunc(&mouseclick);
	glutMotionFunc(&mousemove);
	

	glutMainLoop();
	exit(0);
}
