#include <stdio.h>			// Allows standard io
#include <stdlib.h>			// Needed for malloc and free
#include <unistd.h>			// Sw that the sleep function can be called
#include <GL/glut.h>		// Header for the GLUT library
#include <GL/gl.h>			// Header for the OpenGL32 library
#include <GL/glut.h>		// Header for the GLu32 library
#include "../Render/BuildList.hpp"
#include "../Textures/Textures.hpp"
//#include "../particles/Particles.hpp"

using namespace nuke::shape;
using namespace gfx::rend;
using namespace gfx::tex;
//using namespace nuke::part;

vec3 cubeSize(2.0f, 2.0f, 2.0f);

// Define ascii codes for the keyboard keys
#define ESCAPE		27			// Macro for the escape key
#define PAGE_UP		73
#define PAGE_DN		81
#define UP_ARROW	72			
#define DOWN_ARROW	80			
#define LEFT_ARROW	75			
#define RIGHT_ARROW	77			

const int NUM_PARTICLES = 1;

int window;				    // Number of the GLUT window
int light;					// Lighting (1 = ON, 0 = OFF)
int lp;						// If 'L' is pressed
int fp;						// If 'F' is pressed

GLfloat rotX, rotY, rotZ;			// Angles and speeds of rotation
GLfloat z = -5.0f;					// Depth into the screen
gfxTextures textureArray;
GLuint blend;						// Turn the blending on or off

GLuint cube = 1000;						// Storage for the display list
GLuint top;							// Storage for the second display list
GLuint xloop;						// Loop for the x axis
GLuint yloop;						// Loop for the y axis 

// Define the particles to use for the visual test
// Initially the particles will be drawn without a texture
//Particles<NUM_PARTICLES, Particle, ParticleInitializer, ParticleAction> particles;

// Define colors for the boxes
static GLfloat boxcol[5][3] = {
	{1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
};

// Define colors for the top of the boxes
static GLfloat topcol[5][3] = {
	{.5f,0.0f,0.0f},{0.5f,0.25f,0.0f},{0.5f,0.5f,0.0f},{0.0f,0.5f,0.0f},{0.0f,0.5f,0.5f}
};
/*
void particleConfig()
{
	particles.initializerPolicy.velocityPolicy.SetVelocity(vec3(0.002f, 0.003f, 0.f));	// Set their velocity
	particles.initializerPolicy.shapePolicy.SetSize(vec3(0.5f, 0.5f, 0.5f));		    // Set their size
	particles.initializerPolicy.shapePolicy.SetType(DrawableShape::CUBE);			    // Set the shape
	particles.initializerPolicy.colorPolicy.SetColor(vec4(0.5f, 0.5f, 0.5f, 1.0f));		// Set the color

	// Emit NUM_PARTICLES at the origin
	particles.Emit(NUM_PARTICLES, vec3(0.f, 0.f, -6.f)); 

	// Load a texture for the particles (REQUIRED)
	particles.BindTextures({"textures/Crate.bmp"});

	// Build the shapes for the particles
	particles.BuildShapes();
}
*/
// General OpenGL init. Sets all initial params
void initGL(int width, int height)
{
	//particleConfig();
	textureArray.Append({"textures/Crate.bmp"});
	BuildList(DrawableShape::CUBE, cubeSize, &cube);				//
	glEnable(GL_TEXTURE_2D);					// Enable texture mapping
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

	// Setup a light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
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
	
	glBindTexture(GL_TEXTURE_2D, textureArray.textures[0]);

	// Make a pyramid like structure from cubes
	for (yloop = 1; yloop < 6; yloop++) {
		for (xloop = 0; xloop < yloop; xloop++) {
			glLoadIdentity();

			glTranslatef(1.4f+(((float)xloop)*2.8f)-(((float)yloop)*1.4f),((6.0f-((float)yloop))*2.4f)-7.0f,-20.0f);

			glRotatef(45.0f - (2.0f*yloop)+rotX, 1.0f, 0.0f, 0.0f);
			glRotatef(45.0f + rotY, 0.0f, 1.0f, 0.0f);

			glColor3fv(boxcol[yloop-1]);	    
			glCallList(cube);
	    
			glColor3fv(topcol[yloop-1]);
			glCallList(top);
		}
	}

	//particles.Draw();

	//particles.Update();
	
	// Double buffered so swap the buffers 
	// to display what was just drawn
	glutSwapBuffers();
}

// Function that will check for exit, lighting, and filter
void keyPressed(unsigned char key, int x, int y)
{
	usleep(100);	// No funny behaviour when key presses are too fast

	switch(key) {
		case ESCAPE:						// Close the program
			glutDestroyWindow(window);		// Destroy the GLUT window
			exit(0);
			break;

			default:
			break;
	}
}

// Function for dealing with the movement
void specialKeyPressed(int key, int x, int y)
{
	// Aviod thrashing the key
	usleep(100);

	switch (key) {
		case GLUT_KEY_UP:
				rotX -= 1.f;
				break;
				
		case GLUT_KEY_DOWN:
				rotX += 1.f;
				break;

		case GLUT_KEY_LEFT:
				rotY -= 1.f;
				break;

		case GLUT_KEY_RIGHT:
				rotY += 1.f;		
				break;

		default:
				printf("Special key pressed, but no action yet");
				break;
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

	// Check for the special keys
	glutSpecialFunc(&specialKeyPressed);
	
	// Init GL - the window
	initGL(640, 480);

	// Start the event loop
	glutMainLoop();

	return 1;
}
