/*******************************************************************
		   Hierarchical Multi-Part Model Example
********************************************************************/
#include <Duck.h>

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//#include <gl/glut.h>
//#include <utility>
//#include <vector>
#include "Vectors.h"
#include "CubeMesh.h"
#include "QuadMesh.h"


// so i can use remainder on duckAngle
#include <cmath>
#include <math.h>


const int vWidth = 650;    // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

// Note how everything depends on robot body dimensions so that can scale entire robot proportionately
// just by changing robot body scale
float bodyRadius = 3.0;
float headRadius = 0.6f * bodyRadius;
float beakRadius = 0.6f * headRadius;
float beakLength = 0.7f * headRadius;
float tailRadius = 0.45f * bodyRadius;
float tailLength = 0.5f * bodyRadius;

float upperArmLength = bodyRadius;
float upperArmWidth = 0.125f * bodyRadius;
float gunLength = upperArmLength / 4.0f;
float gunWidth = upperArmWidth;
float gunDepth = upperArmWidth;
float boothLength = 20;
//float stanchionRadius = 0.1* bodyRadius;
//float baseWidth = 2 * bodyRadius;
//float baseLength = 0.25* boothLength;

// Control duck body rotation on base
float duckAngle = 0;
float duckAngle2 = 180;

float amplitude = 0.2f;   // Height of the sine wave
float frequency = 3.0f;   // Controls number of waves
float length = boothLength;      // Length of the wall
float height = 2.0f;      // Height of the wall

///////////////// my attempt to animate the duck left to right
float duckPosX = -7.0f;
float duckPosX2 = -7.0f;
float duckPosY = 0;
float duckDirection = 1;


bool isDuckSpinning = false;


// Control arm rotation
float shoulderAngle = -40.0;
float gunAngle = -25.0;

// Spin Cube Mesh
float cubeAngle = 0.0;

// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat duckBody_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat duckBody_mat_specular[] = { 0.9f,0.9f,0.9f,1.0f };
GLfloat duckBody_mat_diffuse[] = { 0.1f,0.35f,0.1f,1.0f };
GLfloat duckBody_mat_shininess[] = { 150.0F };


GLfloat duckArm_mat_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
GLfloat duckArm_mat_diffuse[] = { 0.5f,0.0f,0.0f,1.0f };
GLfloat duckArm_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat duckArm_mat_shininess[] = { 32.0F };

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.01f,0.0f,0.01f,0.01f };
GLfloat gun_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat booth_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat booth_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat booth_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat booth_mat_shininess[] = { 76.8F };


// Light properties
GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.9F, 0.9F, 0.9F, 1.0F };


// Mouse button
int currentButton;

// A template cube mesh
CubeMesh* cubeMesh = NULL;

// A flat open mesh
QuadMesh* groundMesh = NULL;

// Default Ground Mesh Size
int meshSize = 16;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void drawDuck();
void drawBody();
//void drawHead();
void drawBooth();
//void drawLeftArm();
//void drawRightArm();
void spinUp();
void spinDown();
void LtoR();
void RtoL();

void drawWaterWave();

// this was chatgpt
void drawWaterWave() {
	// Save current OpenGL color and lighting state
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);

	// Disable lighting or color material side effects
	glDisable(GL_LIGHTING);  // <- easiest way to keep color pure
	glColor3f(0.4f, 0.7f, 1.0f);  // light blue water

	float depth = 0.5f;     // how thick the water wall is
	int segments = 100;

	glBegin(GL_QUADS);
	for (int i = 0; i < segments; i++) {
		float x1 = (i / (float)segments) * length - length / 2;
		float x2 = ((i + 1) / (float)segments) * length - length / 2;

		float y1 = sin(frequency * x1) * amplitude + height;
		float y2 = sin(frequency * x2) * amplitude + height;

		// FRONT FACE (z = 0)
		glVertex3f(x1, 0.0f, 0.0f);
		glVertex3f(x2, 0.0f, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x1, y1, 0.0f);

		// BACK FACE (z = -depth)
		glVertex3f(x1, 0.0f, -depth);
		glVertex3f(x2, 0.0f, -depth);
		glVertex3f(x2, y2, -depth);
		glVertex3f(x1, y1, -depth);

		// TOP FACE (connect front & back along sine)
		glColor3f(0.2f, 0.5f, 0.8f);  // darker blue for top
		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y2, 0.0f);
		glVertex3f(x2, y2, -depth);
		glVertex3f(x1, y1, -depth);

		// reset color for next face
		glColor3f(0.4f, 0.7f, 1.0f);
	}
	glEnd();

	// Restore the previous OpenGL lighting/material state
	glPopAttrib();
}


int main(int argc, char** argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("3D Hierarchical Example");

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);

	// Start event loop, never returns
	glutMainLoop();

	return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). 
void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);   // This second light is currently off

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Other initializatuion
	// Set up ground quad mesh
	Vector3 origin = Vector3(-16.0f, 0.0f, 16.0f);
	Vector3 dir1v = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 dir2v = Vector3(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 32.0, 32.0, dir1v, dir2v);

	Vector3 ambient = Vector3(0.0f, 0.05f, 0.0f);
	Vector3 diffuse = Vector3(0.4f, 0.8f, 0.4f);
	Vector3 specular = Vector3(0.04f, 0.04f, 0.04f);
	float shininess = 0.2f;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);

	cubeMesh = new CubeMesh();
	ambient = Vector3(0.05f, 0.00f, 0.0f);
	diffuse = Vector3(0.05f, 0.0f, 0.0f);
	specular = Vector3(0.5f, 0.5f, 0.5f);
	shininess = 4.0;
	cubeMesh->setMaterial(ambient, diffuse, specular, shininess);

}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity(); // M = I
	// Create Viewing Matrix V
	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // M = IV

	// Draw Robot
	// Apply modelling transformations M to move robot
	// ModelView matrix is set to IV, where I is identity matrix
	// M = IV
	drawDuck();
	glPushMatrix();
	glTranslatef(duckPosX, duckPosY, 0);
	glTranslatef(0, -2, 0);
	glRotatef(duckAngle, 0.0, 0.0, -1.0);
	glTranslatef(0, 2, 0);
	drawBody();
	glPopMatrix();
	//drawBody();
	glutTimerFunc(10, animationHandler, 0);


	/*

	// Example of drawing a mesh (closed cube mesh)
	glPushMatrix();
	  // spin cube
	  glTranslatef(8.0, 0, 3.0);
	  glRotatef(cubeAngle, 0.0, 1.0, 0.0);
	  glTranslatef(-8.0, 0, -3.0);
	  // position and draw cube
	  glTranslatef(8.0, 0, 3.0);
	  cubeMesh->drawCubeMesh();
	glPopMatrix();

	*/

	// draw water wave
	glPushMatrix();
	glTranslatef(0.0, -3.0, 0.0);
	drawWaterWave();
	glPopMatrix();

	// Draw ground mesh
	glPushMatrix();
	glTranslatef(0.0, -20.0, 0.0);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	glutSwapBuffers();   // Double buffering, swap buffers
}

void drawDuck()
{
	glPushMatrix(); // copy M = IV and push onto the stack

	//glPushMatrix(); // copy M = IV and push onto the stack
	 // spin entire duck (except base) on base plate. 
	 //glRotatef(duckAngle, 0.0, 0.0, -1.0); // M = I V R_y(duckAngle)




	 //glTranslatef(5, -3, 0); // translate duck left and right
	 //glRotatef(duckAngle, 0.0, 0.0, -1.0); // M = I V R_y(duckAngle)
	 //glTranslatef(-2, duckPosY + 3, 0);

	//glTranslatef(duckPosX, duckPosY, 0);


	 //drawBody();
	 //drawHead();
	 //drawLeftArm();
	 //drawRightArm();
	//glPopMatrix(); // restore M = IV

  // write out the duck coordnates to the console in a formatted string
	printf("Coordinates: duckPosX = %.4f\n", duckPosX);
	printf("Coordinates: duckAngle = %.4f\n\n", duckAngle);

	
	glPopMatrix();


	// don't want to spin fixed base plate for this duck example so this is done outside of 
	// robot push/pop above so it doesn't "inherit" the R_y(duckAngle)
	drawBooth();

	glPopMatrix();
}

void spinDown() {

	glPushMatrix();
	glTranslatef(7, -3, 0); // translate duck left and right
	glRotatef(duckAngle, 0.0, 0.0, -1.0); // M = I V R_y(duckAngle)
	glTranslatef(-7, 3, 0);
	glTranslatef(7, 0, 0);
	//drawBody();
	glPopMatrix();

}


void spinUp() {

	glPushMatrix();
	glTranslatef(-7, -3, 0); // translate duck left and right
	glRotatef(duckAngle2, 0.0, 0.0, -1.0); // M = I V R_y(duckAngle)
	glTranslatef(7, 3, 0);
	glTranslatef(-7, 0, 0);
	//drawBody();
	glPopMatrix();

}

void LtoR() {

	glPushMatrix();
	glTranslatef(duckPosX, 0, 0);
	//drawBody();
	glPopMatrix();

}

void RtoL() {

	glPushMatrix();
	glRotatef(180, 0, 0, -1); // to make the duck face forward	
	glTranslatef(duckPosX2, 6, 0);
	//drawBody();
	glPopMatrix();

}

void drawBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, duckBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, duckBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, duckBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, duckBody_mat_shininess);

	// Isolate scale of duck body
	glPushMatrix();
	//glRotatef(-90, 0, 10, 0);
	glTranslatef(0, 0, 0);
	glScalef(0.3, 0.3, 0.3);

	// duck body
	glPushMatrix(); // M = I V R_y(duckAngle) copy and push onto stack 
	glScalef(bodyRadius, bodyRadius * 0.75, bodyRadius * 0.8); // M = I V R_y(duckAngle) S
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix(); // M = I V R_y(duckAngle)

	// duck head
	glPushMatrix();
	// Position head with respect to its parent part (the body)
	glTranslatef(1.5, (0.5 * bodyRadius + 0.5 * headRadius) + 0.5, 0); // this will be done last
	// "Build" Head (i.e. scale it and draw it)
	glPushMatrix();
	glScalef(headRadius, headRadius, headRadius);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	// duck beak
	glPushMatrix();
	// Position beak with respect to its parent part (the head)
	glRotatef(90, 0, 10, 0);
	glTranslatef(0, (0.5 * headRadius + 0.5 * beakRadius) - 1.5, 1); // this will be done last
	// "Build" Beak (i.e. scale it and draw it)
	glPushMatrix();
	glScalef(beakRadius, beakRadius, beakRadius);
	glutSolidCone(beakRadius, beakLength, 20, 20);
	glPopMatrix();
	// end of duck beak
	glPopMatrix();


	// end of duck head
	glPopMatrix();


	// duck tail
	glPushMatrix();
	// Position tail with respect to its parent part (the body)
	glRotatef(-90, 0, 10, 0);
	glTranslatef(0, (0.5 * bodyRadius + 0.5 * tailRadius) - 1.5, 2); // this will be done last
	glRotatef(-45, 10, 0, 0);
	// "Build" Beak (i.e. scale it and draw it)
	glPushMatrix();
	glScalef(tailRadius, tailRadius * 0.5, tailRadius);
	glutSolidCone(tailRadius, tailLength, 20, 20);
	glPopMatrix();
	// end of duck tail
	glPopMatrix();

	// end of duck body
	glPopMatrix();
}

void drawHead()
{
	// Set robot material properties per body part. Can have seperate material properties for each part
	glMaterialfv(GL_FRONT, GL_AMBIENT, duckBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, duckBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, duckBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, duckBody_mat_shininess);

	glPushMatrix();
	// Position head with respect to its parent part (the body)
	glTranslatef(2, 0.5 * bodyRadius + 0.5 * headRadius, 0); // this will be done last

	// "Build" Head (i.e. scale it and draw it)
	glPushMatrix();
	glScalef(headRadius, headRadius, headRadius);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();

	glPopMatrix();
}


void drawBooth()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, booth_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, booth_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, booth_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, booth_mat_shininess);

	glPushMatrix();
	// Position stanchion and base plate with respect to parent part (the body)
	glTranslatef(0, 0, 0); // this will be done last



	// booth left
	glPushMatrix();
	glTranslatef(-9, 0, 0);
	glScalef(boothLength * 0.1, boothLength, boothLength * 0.2);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	// booth right
	glPushMatrix();
	glTranslatef(9, 0, 0);
	glScalef(boothLength * 0.1, boothLength, boothLength * 0.2);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	// booth top
	glPushMatrix();
	glTranslatef(0, 8, 0);
	glScalef(boothLength + 2, boothLength * 0.2, boothLength * 0.1);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	// booth bottom
	glPushMatrix();
	glTranslatef(0, -8, 0);
	glScalef(boothLength * 0.8, boothLength * 0.5, boothLength * 0.2);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();



	// base plate
	//glPushMatrix();
	// Position base with respect to parent stanchion
	//glTranslatef(0.0, -0.25* boothLength, 0.0);
	// Build base
	//glScalef(baseWidth, baseLength, baseWidth);
	//glRotatef(-90.0, 1.0, 0.0, 0.0);
	//glutSolidCube(1.0);
	//glPopMatrix();

	glPopMatrix();
}

void drawLeftArm()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, duckArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, duckArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, duckArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, duckArm_mat_shininess);

	glPushMatrix();
	// Position arm with respect to parent body
	glTranslatef(0.5 * bodyRadius + 0.5 * upperArmWidth, 0, 0.0); // this will be done last

	// build arm
	glPushMatrix();
	glScalef(upperArmWidth, upperArmLength, upperArmWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();
}

void drawRightArm()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, duckArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, duckArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, duckArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, duckArm_mat_shininess);

	glPushMatrix();

	// Rotate arm at shoulder
	glTranslatef(-(0.5 * bodyRadius + 0.5 * upperArmWidth), 0.5 * upperArmLength, 0.0);
	glRotatef(shoulderAngle, 1.0, 0.0, 0.0);
	glTranslatef((0.5 * bodyRadius + 0.5 * upperArmWidth), -0.5 * upperArmLength, 0.0);

	// Position arm and gun with respect to parent body
	glTranslatef(-(0.5 * bodyRadius + 0.5 * upperArmWidth), 0, 0.0);

	// build arm
	glPushMatrix();
	glScalef(upperArmWidth, upperArmLength, upperArmWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	//  Gun
	glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);

	glPushMatrix();
	// rotate gun
	glTranslatef(-(0.5 * bodyRadius + 0.5 * upperArmWidth), -(0.5 * upperArmLength), 0.0);
	glRotatef(gunAngle, 1.0, 0.0, 0.0);
	glTranslatef((0.5 * bodyRadius + 0.5 * upperArmWidth), (0.5 * upperArmLength), 0.0);

	// Position gun with respect to parent arm 
	glTranslatef(0, -(0.5 * upperArmLength + 0.5 * gunLength), 0.0);

	// build gun
	glScalef(gunWidth, gunLength, gunDepth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPopMatrix();


}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	// Set up viewport, projection, then change to modelview matrix mode - 
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 140.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

bool stop = false;

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 't':
		duckAngle2 += 2.0;
		break;
	case 'r':
		duckAngle += 2.0;
		break;
	case 'R':
		duckAngle -= 2.0;
		break;
	case 'a':
		duckPosX += 0.05;
		break;
	case 'A':
		duckPosX -= 0.05;
		break;
	case 'f':
		duckPosX2 += 0.05;
		break;
	case 'g':
		duckPosY += 0.05;
		break;
	case 'G':
		duckPosY -= 0.05;
		break;
	case 's':
		glutTimerFunc(10, animationHandler, 0);
		break;
	case 'S':
		stop = true;
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}



void animationHandler(int param)
{
	if ((duckAngle == 0.0f || duckAngle >= 360.0f) && duckPosX < 7.0f) {
		duckAngle = 0.0f;
		duckPosY = amplitude * sin(((2.0f * 3.14159f) / frequency) * duckPosX);
		duckPosX += 0.00005;
	}
	else if (duckPosX >= 7.0f && duckAngle < 180.0f) {
		duckAngle += 0.0005;
		duckPosY = 0;
	}
	else if (duckAngle >= 180.0f && duckPosX > -7.0f) {
		duckAngle = 180.0f;
		duckPosX -= 0.00005;
		duckPosY = 0;
	}
	else if (duckPosX <= -7.0f && duckAngle < 360.0f) {
		duckAngle += 0.0005;
		duckPosY = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(10, animationHandler, 0);
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	// Help key
	if (key == GLUT_KEY_F1)
	{

	}
	// Do transformations with arrow keys
	//else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
	//{
	//}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			stop = false;
			//glutTimerFunc(10, animationHandler, 0);


		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			stop = true;

		}
		break;
	default:
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}