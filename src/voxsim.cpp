#include <stdio.h>			
#include <stdlib.h>

#include <GL/glut.h>		// Header for the GLUT library
#include <GL/gl.h>			// Header for the OpenGL32 library
#include <GL/glut.h>		// Header for the GLu32 library
#include <unistd.h>			// So that we can call the sleep funtion

#define ESCAPE	27			// Macro for the escape key

int window;				    // Number of the GLUT window

float thetaTri;				// Angle of rotation for the triangle
float thetaQuad;			// Angle of rotation for the quad

// General OpenGL init. Sets all initial params
void initGL(int width, int height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Black background
	glClearDepth(1.0);							// Allows depth buffer to be cleared
	glDepthFunc(GL_LESS);						// The type of depth test
	glEnable(GL_DEPTH_TEST);					// Enables depth testing
	glShadeModel(GL_SMOOTH); 					// Enables smoothe color shading 

	glMatrixMode(GL_PROJECTION);				// Type of GL Matrix
	glLoadIdentity();							// Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);					// Back to model view matrix
}

// Resize window if (for some reason) the size is changed
void resizeGlScene(int width, int height)
{
	if (height == 0)					// Prevent div by 0 and udef behaviour
		height = 1;

	glViewport(0, 0, width, height);	// Reset current viewport and perspective

	glMatrixMode(GL_PROJECTION);		// Tell GL we want to operate on perspective 
	glLoadIdentity();					// Reset perspective matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);			// Back to model view matrix
}

// Main OpenGL drawing function 
void drawGlScene()
{
	// Clear the color and the depth buffers 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// Reset the view
	glLoadIdentity();

	// Move left 1.5 units and 6.0 units into the screen 
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glRotatef(thetaTri, 0.0f, 1.0f, 0.0f);		// Rotate about y axis

	// Draw a triangle 
	glBegin(GL_POLYGON);				// Start drawing a polygon
	glColor3f(1.0f, 0.0f, 0.0f);		// Top vertex red
	glVertex3f( 0.0f,  1.0f, 0.0f);		// Top
	glColor3f(0.0f, 1.0f, 0.0f);		// Bottom right vertex green
	glVertex3f( 1.0f, -1.0f, 0.0f);		// Bottom right
	glColor3f(0.0f, 0.0f, 1.0f);		// Bottom left vertex blue
	glVertex3f(-1.0f, -1.0f, 0.0f);		// Bottom left
	glEnd();							// Finished drawing a polygon

	glLoadIdentity();						// Reset the view
	glTranslatef(1.5f, 0.0f, -6.0f);		// Move 1.5 units right from the origin		
	glRotatef(thetaQuad, 1.0f, 0.0f, 0.0f);	// Rotate quad about x axis

	// Draw a square 
	glBegin(GL_QUADS);					// Start drawing a quad
	glColor3f(0.0f, 1.0f, 0.0f);		// Top left Green
	glVertex3f(-1.0f,  1.0f, 0.0f);		// Top left
	glColor3f(1.0f, 0.0f, 1.0f);		// Top right magenta
	glVertex3f( 1.0f,  1.0f, 0.0f);		// Top right 
	glColor3f(0.0f, 1.0f, 1.0f);		// Bottom right cyan
	glVertex3f( 1.0f, -1.0f, 0.0f);		// Bottom right
	glColor3f(0.0f, 0.0f, 1.0f);		// Bottom left blue
	glVertex3f(-1.0f, -1.0f, 0.0f);		// Bottom left
	glEnd();							// Finished drawing the quad

	// Update the amount of rotation
	thetaTri  += 0.2f;
	thetaQuad -= 0.7f;

	// Double buffered so swap the buffers 
	// to display what was just drawn
	glutSwapBuffers();
}

// Function that will check if the escape key is pressed and then exit
void keyPressed(unsigned char key, int x, int y)
{
	usleep(100);	// No funny behaviour when key presses are too fast

	// If the key is the escape key, exit
	if (key == ESCAPE) {
		glutDestroyWindow(window);		// Destroy the GLUT window
		exit(0);
	}
}

int main(int argc, char **argv) 
{
	// Init GLUT state. GLUT will take any command line  
	// args that pertain to either it or to X windows 
	glutInit(&argc, argv);		

	// Select the display mode for GLUT
	// [RGBA, Double buffer, Alpha channel enabled, Depth buffer]
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	// Define a 640 x 480 window
	glutInitWindowSize(640, 480);

	// Make the window co-ords be top left of screen
	glutInitWindowPosition(0, 0);

	// Open a window
	window = glutCreateWindow("VoxSim V0.1 =D");

	// OpenGL drawing function init
	glutDisplayFunc(&drawGlScene);

	// Now make the window fullscreen (prev stuff must be done first)
	glutFullScreen();

	// Redraw the gl scene regardless of the presence of gl events
	glutIdleFunc(&drawGlScene);

	// Init the function called when resizing the screen
	glutReshapeFunc(&resizeGlScene);

	// Register the keyboard checking function
	glutKeyboardFunc(&keyPressed);

	// Init GL - the window
	initGL(640, 480);

	// Start the event loop
	glutMainLoop();

	return 1;
}
