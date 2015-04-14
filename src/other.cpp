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

int window;				    // Number of the GLUT window
int light;					// Lighting (1 = ON, 0 = OFF)
int lp;						// If 'L' is pressed
int fp;						// If 'F' is pressed

GLfloat rotX, rotY, speedX, speedY;			// Angles and speeds of rotation
GLfloat z = -5.0f;							// Depth into the screen

GLfloat lightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f};	// White ambient light half density
GLfloat lightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};	// Really bright white light
GLfloat lightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};	// Out the screen facing front face

GLuint fogMode[] = {GL_EXP, GL_EXP2, GL_LINEAR};	// Storage for the fog types
GLuint fogfilter = 0;								// Which type of fog to use
GLfloat fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f};		// Fog color

GLuint filter;						
GLuint texture[1];					// Storage for 1 textures
GLuint blend;						// Turn the blending on or off

// Define an image type 
typedef struct Image {
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
	
	const char * textureFile = "textures/Crate.bmp";		// Declare the texture filename

	if (!loadImage(textureFile, image1)) {
		exit(1);
	}

	// One texture
	glGenTextures(1, &texture[0]);

	// Create a mipmapped texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	// Mipmapped
	
	// Build the mipmapped texture, params are:
	//	[2D texture, 3 colors, wifth, height, RGB, byte type data, actual data]
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

	// Free the image 
	if (image1) {
		if (image1->data) {
			free(image1->data);
		}
		free(image1);
	}
}

// General OpenGL init. Sets all initial params
void initGL(int width, int height)
{
	loadGlTextures();							// Load the textures
	glEnable(GL_TEXTURE_2D);					// Enable texture mapping
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Black background
	glClearDepth(1.0);							// Allows depth buffer to be cleared
	glDepthFunc(GL_LESS);						// The type of depth test
	glEnable(GL_DEPTH_TEST);					// Enables depth testing
	glShadeModel(GL_SMOOTH); 					// Enables smoothe color shading 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glMatrixMode(GL_PROJECTION);				// Type of GL Matrix
	glLoadIdentity();							// Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);					// Back to model view matrix

	// Setup a light
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);	// Add ambient light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);	// Add diffusion light
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);	// Set the light position
	glEnable(GL_LIGHT1);

	// Setup the fog
	glFogi(GL_FOG_MODE, fogMode[fogfilter]);       // Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);               // Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.35f);                 // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);			       // Fog Hint Value
	glFogf(GL_FOG_START, 4.0f);                    // Fog Start Depth
	glFogf(GL_FOG_END, 9.0f);                      // Fog End Depth
	glEnable(GL_FOG);                              // Enables GL_FOG	
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
	glLoadIdentity();						// Reset the vie
	glTranslatef(0.f, 0.0f, z);				// Move z units along z axis
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);		// Rotate x-axis
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);		// Rotate y-axis

	glBindTexture(GL_TEXTURE_2D, texture[filter]);	// Select the texture to use

	// Draw a square 
	glBegin(GL_QUADS);					// Start drawing a quad

	 // Front Face (note that the texture's corners have to match the quad's corners)
    glNormal3f( 0.0f, 0.0f, 1.0f);                              // front face points out of the screen on z.
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
    
    // Back Face
    glNormal3f( 0.0f, 0.0f,-1.0f);                              // back face points into the screen on z.
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
	
    // Top Face
    glNormal3f( 0.0f, 1.0f, 0.0f);                              // top face points up on y.
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    
    // Bottom Face       
    glNormal3f( 0.0f, -1.0f, 0.0f);                             // bottom face points down on y. 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    
    // Right face
    glNormal3f( 1.0f, 0.0f, 0.0f);                              // right face points right on x.
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);                              // left face points left on x.
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
	
	glEnd();							// Finished drawing the quad

	rotX += speedX;
	rotY += speedY;

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

		case 76:
		case 108:							// Switch the lighting
			light = light ? 0 : 1;			// Change light variable
			if (!light) {
				glDisable(GL_LIGHTING);		
			}
			else {
				glEnable(GL_LIGHTING);
			}
			break;

		case 70:
		case 102:							// Switch the filter
			filter +=1;
			if (filter > 2) {				// Out of range
				filter = 0;					// back to first filter
			}
			break;

		case 'g':
		case 'G':
			fogfilter += 1;
			if (fogfilter > 2) {
				fogfilter = 0;
			}
			glFogi(GL_FOG_MODE, fogMode[fogfilter]);
			break;

		case 98:
		case 110:							// Switch the blending
			blend = blend ? 0 : 1;			// Toggle current blend value
			if (!blend) {
				glDisable(GL_BLEND);		// Turn off blending
				glEnable(GL_DEPTH_TEST);	// Turn on depth testing
			}
			else {
				glEnable(GL_BLEND);			// Turn on blending
				glDisable(GL_DEPTH_TEST);	// Turn off depth testing
			}
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
		case GLUT_KEY_PAGE_UP:
			z -= 0.2f;
			break;

		case GLUT_KEY_PAGE_DOWN:
			z += 0.2f;
			break;

		case GLUT_KEY_UP:
			speedX += 0.1f;
			break;

		case GLUT_KEY_DOWN:
			speedX -= 0.1f;
			break;

		case GLUT_KEY_LEFT:
			speedY -= 0.1f;
			break;

		case GLUT_KEY_RIGHT: 
			speedY += 0.1f;
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
