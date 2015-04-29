#include <stdio.h>			// Allows standard io
#include <stdlib.h>			// Needed for malloc and free
#include <unistd.h>			// Sw that the sleep function can be called
#include <GL/glut.h>		// Header for the GLUT library
#include <GL/gl.h>			// Header for the OpenGL32 library
#include <GL/glut.h>		// Header for the GLu32 library
#include "../Render/BuildList.hpp"
#include "../Texture/Textures.hpp"
#include "../Particles/Particles.hpp"
#include "../Particles/ParticleTypedefs.h"
#include "../Particles/ParticleSimulations.hpp"

using namespace gfx::rend;
using namespace gfx::tex;
using namespace gfx::part;

vec3 cubeSize(2.0f, 2.0f, 2.0f);

// Define ascii codes for the keyboard keys
#define ESCAPE		27			// Macro for the escape key
#define PAGE_UP		73
#define PAGE_DN		81
#define UP_ARROW	72			
#define DOWN_ARROW	80			
#define LEFT_ARROW	75			
#define RIGHT_ARROW	77			

const int NUM_PARTICLES = 10000;

int window;				    // Number of the GLUT window
int light;					// Lighting (1 = ON, 0 = OFF)
int lp;						// If 'L' is pressed
int fp;						// If 'F' is pressed

GLfloat rotX = 0.0f, rotY = 0.0f, rotZ;			// Angles and speeds of rotation
GLfloat z = -5.0f;					// Depth into the screen
gfxTextures textureArray;
GLuint blend;						// Turn the blending on or off

GLuint cube, plane;					// Storage for the display list
GLuint top;							// Storage for the second display list
GLuint xloop;						// Loop for the x axis
GLuint yloop;						// Loop for the y axis 

gfxParticles<NUM_PARTICLES, gfxParticle, ParticleInitializer, ParticleAction> particles;

// General OpenGL init. Sets all initial params
void initGL(int width, int height)
{
	BurstSimulationSetup(particles);
	BuildList( gfx::shape::DrawableShapes::PLANE, vec3(10.f, 0.0f, 10.f), &plane );
	glEnable( GL_TEXTURE_2D );					// Enable texture mapping
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );		// Black background
	glClearDepth( 1.0 );						// Allows depth buffer to be cleared
	glDepthFunc( GL_LESS );						// The type of depth test
	glEnable( GL_DEPTH_TEST );					// Enables depth testing
	glShadeModel( GL_SMOOTH ); 					// Enables smoothe color shading 

	glMatrixMode(GL_PROJECTION);				// Type of GL Matrix
	glLoadIdentity();							// Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective( 45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f );

	glMatrixMode( GL_MODELVIEW );					// Back to model view matrix

	// Setup a light
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );
	glEnable( GL_COLOR_MATERIAL );
}

// Resize window if (for some reason) the size is changed
void resizeGlScene( int width, int height )
{
	if ( height == 0 )	{				// Prevent div by 0 and udef behaviour
		height = 1;
	}

	glViewport( 0, 0, width, height );	// Reset current viewport and perspective

	glMatrixMode( GL_PROJECTION );		// Tell GL we want to operate on perspective 
	glLoadIdentity();					// Reset perspective matrix

	// Calculate the aspect ratio of the window
	gluPerspective( 45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f );
	glMatrixMode( GL_MODELVIEW );			// Back to model view matrix
}

// Main OpenGL drawing function 
void drawGlScene()
{
	// Clear the color and the depth buffers 
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	
	
	// Draw the particles to the screen
	particles.Draw();

	// Update the particle attributes
	particles.Update();

	glLoadIdentity();
	glTranslatef(0.0f, -2.5f, -10.f);
	glRotatef(30.f, 0.0f, 1.0f, 0.0f);
	glColor4f( 0.0f, 0.73f, 0.7f, 1.0f);
	glCallList( plane );
	
	// Double buffered so swap the buffers 
	// to display what was just drawn
	glutSwapBuffers();
}

// Function that will check for exit, lighting, and filter
void keyPressed( unsigned char key, int x, int y )
{
	usleep( 100 );	// No funny behaviour when key presses are too fast

	switch( key ) {
		case ESCAPE:						// Close the program
			glutDestroyWindow( window );		// Destroy the GLUT window
			exit(0);
			break;

			default:
			break;
	}
}

// Function for dealing with the movement
void specialKeyPressed( int key, int x, int y )
{
	// Aviod thrashing the key
	usleep( 100 );

	switch ( key ) {
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
				printf( "Special key pressed, but no action yet" );
				break;
	}
}

int main( int argc, char **argv ) 
{
	// Init GLUT state. GLUT will take any command line  
	// args that pertain to either it or to X windows 
	glutInit( &argc, argv );		

	// Select the display mode for GLUT
	// [RGBA, Double buffer, Alpha channel enabled, Depth buffer]
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH );

	// Define a 640 x 480 window
	glutInitWindowSize( 640, 480 );

	// Make the window co-ords be top left of screen
	glutInitWindowPosition( 0, 0 );

	// Open a window
	window = glutCreateWindow( "GFX Test =D" );

	// OpenGL drawing function init
	glutDisplayFunc( &drawGlScene );

	// Now make the window fullscreen (prev stuff must be done first)
	glutFullScreen();

	// Redraw the gl scene regardless of the presence of gl events
	glutIdleFunc( &drawGlScene );

	// Init the function called when resizing the screen
	glutReshapeFunc( &resizeGlScene );

	// Register the keyboard checking function
	glutKeyboardFunc( &keyPressed );

	// Check for the special keys
	glutSpecialFunc( &specialKeyPressed );
	
	// Init GL - the window
	initGL( 640, 480 );

	// Start the event loop
	glutMainLoop();

	return 1;
}
