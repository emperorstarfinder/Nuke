#include <stdio.h>			// Allows standard io
#include <stdlib.h>			// Needed for malloc and free
#include <unistd.h>			// Sw that the sleep function can be called
#include <GL/glut.h>		// Header for the GLUT library
#include <GL/gl.h>			// Header for the OpenGL32 library
#include <GL/glut.h>		// Header for the GLu32 library
#include "../renderer/buildlists.hpp"
#include "../texture/texture.hpp"

using namespace nuke::rend;
using namespace nuke::tex;

vec3 cubeSize(2.0f, 2.0f, 2.0f);

// Define ascii codes for the keyboard keys
#define ESCAPE		27			// Macro for the escape key
#define PAGE_UP		73
#define PAGE_DN		81
#define UP_ARROW	72			
#define DOWN_ARROW	80			
#define LEFT_ARROW	75			
#define RIGHT_ARROW	77			

int window;				    // Number of the GLUT window
int light;					// Lighting (1 = ON, 0 = OFF)
int lp;						// If 'L' is pressed
int fp;						// If 'F' is pressed

GLfloat rotX, rotY, rotZ;			// Angles and speeds of rotation
GLfloat z = -5.0f;					// Depth into the screen
GLuint texture[1];					// Storage for 1 texture
GLuint blend;						// Turn the blending on or off

GLuint cube;						// Storage for the display list
GLuint top;							// Storage for the second display list
GLuint xloop;						// Loop for the x axis
GLuint yloop;						// Loop for the y axis 

// Define colors for the boxes
static GLfloat boxcol[5][3] = {
	{1.0f,0.0f,0.0f},{1.0f,0.5f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,1.0f,1.0f}
};

// Define colors for the top of the boxes
static GLfloat topcol[5][3] = {
	{.5f,0.0f,0.0f},{0.5f,0.25f,0.0f},{0.5f,0.5f,0.0f},{0.0f,0.5f,0.0f},{0.0f,0.5f,0.5f}
};

// Define an image type 
typedef struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
} Image;

/*
// Other bitmap loader not working
static unsigned int getint(FILE *fp)
{
	int c, c1, c2, c3;

	// Get 4 bytes
	c  = getc(fp);
	c1 = getc(fp);
	c2 = getc(fp);
	c3 = getc(fp);

	// Return the result
	return ((unsigned int) c ) + 
		   (((unsigned int) c1) << 8) +
		   (((unsigned int) c2) << 16) +
		   (((unsigned int) c3) << 24);
}

static unsigned short getshort(FILE *fp)
{
	int c, c1;

	// Get 2 bytes
	c  = getc(fp);
	c1 = getc(fp);

	// Return the result
	return ((unsigned int) c) +
		   (((unsigned int) c1) << 8);
}

// Simple bitmap loader
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int loadImage(const char * filename, Image * image)
{
	FILE *file;									// File handle
	unsigned long size;							// Image size (in bytes)
	unsigned long i;							// Counter
	unsigned short int planes;					// Number of planes in image (must be 1)
	unsigned short int bpp;						// Bits per pixel (must be 24)
	char temp;									// Temp col storage for bgr->rgb conversion

	// Make sure that the file exists
	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	
	// Seek through the BMP header
	fseek(file, 18, SEEK_CUR);

	// Read the width
    image->sizeX = getint (file);
    printf("Width of %s: %lu\n", filename, image->sizeX);
    
    // Read the height 
    image->sizeY = getint (file);
    printf("Height of %s: %lu\n", filename, image->sizeY);

	// Calculate the size (assuming 24 bits and 3 bytes per pixel
	size = image->sizeX * image->sizeY * 3;

    // Read the planes
    planes = getshort(file);
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    // Read the bpp
    bpp = getshort(file);
    if (bpp != 24) {
      printf("Bpp from %s is not 24: %u\n", filename, bpp);
      return 0;
    }

	// Seek past the rest of the bitmap header
	fseek(file, 24, SEEK_CUR);

	// Read in the pixel data 
	image->data = (char *)malloc(size);			// Allocate some space
	if (image->data == NULL) {
		printf("Error allocating the memory for the color-corrected image data");
		return 0;
	}

	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading the image data from %s.\n", filename);
		return 0;
	}

	// Reverse all if the colors (bgr -> rgb)
	for (i = 0; i < size; i+=3) {				// Each pixel
		temp = image->data[i];					// blue as temp
		image->data[i] = image->data[i+2];		// Blue to red
		image->data[i+2] = temp;				// Red to blue
	}

	// Success =D
	return 1;
}
*/
// Load the bitmaps and convert them to textures
void loadGlTextures()
{
	Texture image1("textures/Cube.bmp");

	glGenTextures(1, &texture[0]);							// Create the texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// 2D texture (x, y)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					// Scale cheaply when img bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	// Scale cheaply when img smaller than texture

	// Params are:
	//	[2d tex  ,  detail level     ,  num components, img x size, img y size, border,
	//	 col data, unsigned byte data, data itself]
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1.SizeX(), image1.SizeY(), GL_RGB, GL_UNSIGNED_BYTE, image1.Data());
}

// General OpenGL init. Sets all initial params
void initGL(int width, int height)
{
	loadGlTextures();							// Load the textures
	//buildLists();								// Build the lists
	BuildCubeList(cubeSize, &cube);				//
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
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);

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
