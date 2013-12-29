#include <windows.h>
#include <time.h>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
	#include <OpenGL/OpenGL.h>
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif


#include "particle_system.h"
#include "SOIL.h"
#include "constants.h"
#include "cloud.h"

using namespace std;

particle_system p(NUMBER_OF_PARTICLES);
float win_width = 400, win_height = 400;

//Called when a key is pressed
void handle_keypress(unsigned char key, int x, int y)
{
    switch(key)
    {
	case 'a':
	case 'A':
		p.add_particles(10);
		break;
	
	case 'd':
	case 'D':
		p.delete_particles(10);
		break;

	case 's':
	case 'S':
		SOIL_save_screenshot("awesome.bmp",SOIL_SAVE_TYPE_BMP,0, 0, 1024, 768);
		break;

    case 27:
        exit(0);
		break;
    }
    glutPostRedisplay();
}



//Initializes 3D rendering
void init()
{
	// Make big points and wide lines
	glPointSize(3);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//Called when the window is resized
void handle_resize(int w, int h)
{
	//setup windows width and height
	win_width = (w==0) ? 1 : w;
	win_height = (h==0) ? 1 : h;

    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport(0, 0, win_width, win_height);

	//Switch to setting the camera perspective
    glMatrixMode(GL_PROJECTION);

    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
	glOrtho(-LENGTH, LENGTH, -LENGTH, LENGTH, -LENGTH, LENGTH);
	
	glMatrixMode(GL_MODELVIEW);
}


//Draws the 3D scene
void draw()
{
	glLoadIdentity();
	
	//Draw particles
	glPushMatrix();
		p.advance(DELTA);
		p.draw();
	glPopMatrix();

	//Draw overlaying quad for trail
	glColor4f(0, 0, 0, 0.1);
	glBegin(GL_QUADS);
		glVertex3f(-LENGTH, -LENGTH, 100);
		glVertex3f(LENGTH, -LENGTH, 100);
		glVertex3f(LENGTH, LENGTH, 100);
		glVertex3f(-LENGTH, LENGTH, 100);
	glEnd();

	glutSwapBuffers();
	glutPostRedisplay();
}


//Handle mouse movement
void mouse_movement(int x, int y ){
	float ww_ratio = float(x)/win_width;
	float wh_ratio = float(y)/win_height;

	p.set_gravity(vec3d((2*ww_ratio-1)*LENGTH, (1-2*wh_ratio)*LENGTH, 0));
}



int main(int argc, char** argv)
{
	srand(time(0));
    p.set_gravity();

	//Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height); //Set the window size

    //Create the window and initialize OpenGL
    glutCreateWindow("Particle System");
    init();

    //Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(draw);
    glutKeyboardFunc(handle_keypress);
    glutReshapeFunc(handle_resize);
	glutPassiveMotionFunc(mouse_movement);
	glutFullScreen();
    
	glutMainLoop();
    return 0; //This line is never reached
}