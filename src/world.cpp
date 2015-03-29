#include <stdio.h>			// Allows standard io
#include <stdlib.h>			// Needed for malloc and free
#include <unistd.h>			// So that the sleep function can be called
#include <math.h>			// Need the trig functions
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

int light = 0;				// Lighting (on/off) default = off
int blend = 0;				// Blending (on/off) default = off

GLfloat xrot;						// X rotation
GLfloat yrot;						// Y rotation
GLfloat speedX;						// Speed of rotX
GLfloat speedY;						// Speed of rotY

GLfloat walkbias = 0;				// For the head bobbing
GLfloat walkbiasangle = 0;	

GLfloat lookupdown = 0.0;

// Define the radin<->degree conversion 
const float piover180 = 0.0174532925f;

float heading, xpos, zpos;

GLfloat camx, camy, camz;			// Cameraposition variables
GLfloat throtate;					// Rotation

GLfloat zoom = 0.0f;				// Depth into the screen

GLfloat lightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f}; 
GLfloat lightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat lightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};

GLuint loop;						// Variable for the loop number
GLuint texture[3];					// Storage for 3 textures

GLuint filter = 0;					// Variable for chosing the texture

// Define the vectex structure
typedef struct {
	GLfloat x, y, z;		// 3D coordinates
	GLfloat u, v;			// Texture coords
} VERTEX;

// Define the triangle structure 
typedef struct {
	VERTEX vertex[3];		// 3 Vertex array
} TRIANGLE;

// Define the sector struct
typedef struct {
	int numtriangles;		// Number of triangles in the sector
	TRIANGLE *triangle;		// Pointer to the triangle array
} SECTOR;


// Define a sector for the world
SECTOR sector;

// Struct for an Image
typedef struct {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
} Image;


// Degree to radian function
float rad(float angle) {
	return angle * piover180;
}


// Helper function for reading the world  from a file.
// Deals with comments and empty lines.
void readstr(FILE * f, char *string)
{
	do {
		fgets(string, 255, f);			// Read a line 
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}

// Function for loading the world from the file 
void setupWorld() 
{
	float x, y, z, u, v;				// Variables to read from file
	int vert, numtriangles;		
	FILE *worldfile;					// Name of te file to load the world from
	char line[256];					// Buffer for a line of the file

	// Open the file 
	worldfile = fopen("maps/world.txt", "rt");

	// Get a relevant line from the file 
	readstr(worldfile, line);
	sscanf(line, "NUMPOLLIES %d\n", &numtriangles);

	// Set the values for the sector 
	sector.numtriangles = numtriangles;
	sector.triangle = (TRIANGLE *) malloc(sizeof(TRIANGLE)*numtriangles); // Allocate some mem for triangles

	// Now read all the triangles from the file
	for (loop = 0; loop < numtriangles; loop++) {
		for(vert = 0; vert < 3; vert++) {
			readstr(worldfile, line);								// Read in a relevant line 
			sscanf(line, "%f %f %f %f %f", &x, &y, &z, &u, &v);		// Save the params
			sector.triangle[loop].vertex[vert].x = x;
			sector.triangle[loop].vertex[vert].y = y;
			sector.triangle[loop].vertex[vert].z = z;
			sector.triangle[loop].vertex[vert].u = u;
			sector.triangle[loop].vertex[vert].v = v;
		}
	}
	fclose(worldfile);												// Close the file
	return;
}

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
	
	const char * textureFile = "textures/Crate.bmp";		// Declare the filename

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

	// Linar filter section
    glBindTexture(GL_TEXTURE_2D, texture[1]);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,	0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
	
	// Mipmapped section
    glBindTexture(GL_TEXTURE_2D, texture[2]);   	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
}

// General OpenGL init. Sets all initial params
void initGL(int width, int height)
{
	loadGlTextures();							// Load the textures
	glEnable(GL_TEXTURE_2D);					// Enable texture mapping
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);				// Translucent blending function
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Black background
	glClearDepth(1.0);							// Allows depth buffer to be clearedI
	glDepthFunc(GL_LESS);						// Type of the depth test
	glEnable(GL_DEPTH_TEST);					// Enable the depth testing
	glShadeModel(GL_SMOOTH); 					// Enables smoothe color shading 

	glMatrixMode(GL_PROJECTION);				// Type of GL Matrix
	glLoadIdentity();							// Reset the projection matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);					// Back to model view matrix

	// Setup the lights
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT1);	
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
	GLfloat xm, ym, zm, um, vm;								// Temp variables
	GLfloat xtrans, ytrans, ztrans;							// Temp transformations
	GLfloat sceneroty;										// Y axis screen rotation
	int numtriangles;

	// Do some calculations 
	xtrans = -xpos;
	ztrans = -zpos;
	ytrans = -walkbias - 0.25f;								// Gead bobbing 
	sceneroty = 360.f - yrot;

	// Clear the color and the depth buffers 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();

	glRotatef(lookupdown, 1.0f, 0.f, 0.f);
	glRotatef(sceneroty, 0.f, 1.0f, 0.f);

	glTranslatef(xtrans, ytrans, ztrans);

	glBindTexture(GL_TEXTURE_2D, texture[filter]);				// Select the texture to use

	// Get the number of triangles
	numtriangles = sector.numtriangles;

	// Go through all the triangles
	for (loop = 0; loop < numtriangles; loop++) {
		glBegin(GL_TRIANGLES);
		glNormal3f(0.f, 0.f, 1.0f);							// Define the normal 

		xm = sector.triangle[loop].vertex[0].x;
		ym = sector.triangle[loop].vertex[0].y;
		zm = sector.triangle[loop].vertex[0].z;
		um = sector.triangle[loop].vertex[0].u;
		vm = sector.triangle[loop].vertex[0].v;
		glTexCoord2f(um,vm); 
		glVertex3f(xm,ym,zm);
	
		xm = sector.triangle[loop].vertex[1].x;
		ym = sector.triangle[loop].vertex[1].y;
		zm = sector.triangle[loop].vertex[1].z;
		um = sector.triangle[loop].vertex[1].u;
		vm = sector.triangle[loop].vertex[1].v;
		glTexCoord2f(um,vm); 
		glVertex3f(xm,ym,zm);
	
		xm = sector.triangle[loop].vertex[2].x;
		ym = sector.triangle[loop].vertex[2].y;
		zm = sector.triangle[loop].vertex[2].z;
		um = sector.triangle[loop].vertex[2].u;
		vm = sector.triangle[loop].vertex[2].v;
		glTexCoord2f(um,vm); 
		glVertex3f(xm,ym,zm);		

		glEnd();
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

		case 'b':
		case 'B':							// Switch the blending
			blend = blend ? 0 : 1;			// Swap the blend value
			if (blend) {
				glEnable(GL_BLEND);
				glDisable(GL_DEPTH_TEST);
			} 
			else {
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
			}
			break;

		case 'f':
		case 'F':							// Swap the filter
			filter++;
			if (filter > 2) {
				filter = 0;
			}
			break;

		case 'l':
		case 'L':							// Swap the lighting	
			light = light ? 0 : 1;
			if (light) {
				glEnable(GL_LIGHTING);
			}
			else {
				glDisable(GL_LIGHTING);
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
			zoom -= 0.2f;				// Zoom out 
			lookupdown -= 0.2f;			// Tilt up
			break;

		case GLUT_KEY_PAGE_DOWN:		
			zoom += 0.2f;				// Zoom in
			lookupdown += 1.0f;			// Tilt down
			break;

		case GLUT_KEY_UP:				// Walk forward, bob head
			xpos -= (float)sin(yrot * piover180) * 0.05f;
			zpos -= (float)cos(yrot * piover180) * 0.05f;
			if (walkbiasangle > 259.0f)
				walkbiasangle = 0.0f;
			else
				walkbiasangle += 10.f;
			walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
			break;

		case GLUT_KEY_DOWN:				// Tilt back, bob head
			xpos += (float)sin(yrot * piover180) * 0.05f;
			zpos += (float)cos(yrot * piover180) * 0.05f;
			if (walkbiasangle < 1.0f)
				walkbiasangle = 359.f;
			else
				walkbiasangle -= 10.f;
			walkbias = (float)sin(walkbiasangle * piover180) / 20.f;
			break;

		case GLUT_KEY_LEFT:				// Look left
			yrot += 1.5f;
			break;

		case GLUT_KEY_RIGHT:
			yrot -= 1.5f;
			break;

		default:
			break;
	}
}

int main(int argc, char **argv) 
{
	// Load the world from disk
	setupWorld();

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
	window = glutCreateWindow("Test World");

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
