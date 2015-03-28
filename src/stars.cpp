#include <stdio.h>			// Allows standard io
#include <stdlib.h>			// Needed for malloc and free
#include <unistd.h>			// So that the sleep function can be called
#include <GL/glut.h>		// Header for the GLUT library
#include <GL/gl.h>			// Header for the OpenGL32 library
#include <GL/glut.h>		// Header for the GLu32 library

// Define ascii codes for the keyboard keys
#define ESCAPE		27			// Macro for the escape key
#define PAGE_UP		73
#define PAGE_DN		81
#define UP_ARROW	72			
#define DOWN_ARROW	80			
#define LEFT_ARROW	75			
#define RIGHT_ARROW	77	

// The number of stars that we will draw
#define NUM_STARS 50

int window;				    // Number of the GLUT window
int twinkle;				// If the stars should twinkle

// Define an image type 
typedef struct {
	int r, g, b;					// Star color
	GLfloat dist;					// Star distance from origin
	GLfloat angle;					// Star angle of rotation
} Stars;

Stars stars[NUM_STARS];				// Allocate some space for the stars

GLfloat zoom = -15.0f;				// Viewing distance from the stars
GLfloat tilt = 90.f;				// The tilt of the view
GLfloat spin;						// Spin for the stars

GLuint loop;						// Variable for the loop number
GLuint texture[1];					// Texture for the stars

// Struct for an Image
typedef struct {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
} Image;

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

// Load the bitmaps and convert them to textures
void loadGlTextures()
{
	Image *image1;											// Load a texture

	// Allocate memory for the texture
	image1 = (Image *)malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for the image\n");
		exit(0);											// Quit
	}
	
	const char * textureFile = "textures/Stars.bmp";		// Declare the filename

	if (!loadImage(textureFile, image1)) {
		exit(1);
	}

	glGenTextures(3, &texture[0]);							// Create the texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// 2D texture (x, y)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Scale cheaply when img bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Scale cheaply when img smaller than texture

	// Params are:
	//	[2d tex  ,  detail level     ,  num components, img x size, img y size, border,
	//	 col data, unsigned byte data, data itself]
	glTexImage2D(GL_TEXTURE_2D,	0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
}

// General OpenGL init. Sets all initial params
void initGL(int width, int height)
{
	loadGlTextures();							// Load the textures
	glEnable(GL_TEXTURE_2D);					// Enable texture mapping
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Black background
	glClearDepth(1.0);							// Allows depth buffer to be cleared
	glShadeModel(GL_SMOOTH); 					// Enables smoothe color shading 

	glMatrixMode(GL_PROJECTION);				// Type of GL Matrix
	glLoadIdentity();							// Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);					// Back to model view matrix

	// Setup blending 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);				// Translucent blending function
	glEnable(GL_BLEND);								// Enable blending
	

	// Setup the stars
	for (loop = 0; loop < NUM_STARS; loop++) {
		stars[loop].angle = 0.0f;					// No rotation at the begining
		stars[loop].dist = loop / NUM_STARS * 5.f;	// 5 comes from the chosen zoom	
		stars[loop].r = rand() % 256;				// Random red
		stars[loop].g = rand() % 256;				// Random green
		stars[loop].b = rand() % 256;				// Random blue
	}
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
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// Select the texture to use

	// Loop through all the stars 
	for (loop = 0; loop < NUM_STARS; loop++) {
		glLoadIdentity();									// Reset the view for each star
		glTranslatef(0.0f, 0.0f, zoom);						// Zoom into the screen
		glRotatef(tilt, 1.0f, 0.0f, 0.0f);					// Rotate tile degrees around x axis

		glRotatef(stars[loop].angle, 0.0f, 1.0f, 0.0f);		// Rotate by start angle 
		glTranslatef(stars[loop].dist, 0.0f, 0.0f);			// move the star along x

		glRotatef(-stars[loop].angle, 0.0f, 1.0f, 0.0f);	// Cancel the rotation
		glRotatef(-tilt, 1.0f, 0.0f, 0.0f);					// Cancel screen tilt

		// If twinkling is enables draw another star
		if (twinkle) {
			glColor4ub(stars[NUM_STARS - loop].r, stars[NUM_STARS -loop].g, stars[NUM_STARS - loop].b, 255);

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}

		// Now draw the main star
		glRotatef(spin, 0.0f, 0.0f, 1.0f);					// Rotate about z axis

		glColor4ub(stars[loop].r, stars[loop].g, stars[loop].b, 255);
		glBegin(GL_QUADS);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
	    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, 0.0f);
	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();

		spin += 0.01f;										// Make the stars spin
		stars[loop].angle += loop / NUM_STARS;				// Change the star angle
		stars[loop].dist  -= 0.01f;							// Bring the stars back to the center

		// Check that the stars are in range
		if (stars[loop].dist < 0.0f) {
			stars[loop].dist += 5.0f;						// Move it back in
			stars[loop].r = rand() % 256;					// New red
			stars[loop].g = rand() % 256;					// New green
			stars[loop].b = rand() % 256;					// New blue
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

		case 84:
		case 116:							// Switch the twinkle
			twinkle = twinkle ? 0 : 1;		// Swap the twinkle value
			break;
	
		default:
			break;
	}
}

// Function to check for movement 
void specialKeyPressed(int key, int x, int y)
{
	usleep(100);						// Again no funny behaviour

	switch(key) {
		case GLUT_KEY_PAGE_UP:			// Zoom out
			zoom -= 0.2f;
			break;

		case GLUT_KEY_PAGE_DOWN:		// Zoom in
			zoom += 0.2f;
			break;

		case GLUT_KEY_UP:				// Tilt up
			tilt += 0.5f;
			break;

		case GLUT_KEY_DOWN:				// Tilt down
			tilt -= 0.5f;
			break;

		default:
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

	// Check the movement functions 
	glutSpecialFunc(&specialKeyPressed);
	
	// Init GL - the window
	initGL(640, 480);

	// Start the event loop
	glutMainLoop();

	return 1;
}
