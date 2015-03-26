#include <stdio.h>			// Allows standard io
#include <stdlib.h>			// Needed for malloc and free
#include <unistd.h>			// So that the sleep function can be called
#include <GL/glut.h>		// Header for the GLUT library
#include <GL/gl.h>			// Header for the OpenGL32 library
#include <GL/glut.h>		// Header for the GLu32 library

#define ESCAPE	27			// Macro for the escape key

int window;				    // Number of the GLUT window

float rotCx, rotCy, rotCz, rotT;				// Angles of rotation for the cube ad triangle

unsigned int texture[1];									// Storage for one texture

// Define an image type 
typedef struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
} Image;

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

	// Read the bmp width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading the width from : %s\n", filename);
	}
	printf("Got the width from %s as : %lu\n", filename, image->sizeX);

	// Read the height 
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading the height from %s\n", filename);
	}
	printf("Got the height from %s as : %lu\n", filename, image->sizeY);

	// Calculate the size (assuming 24 bits and 3 bytes per pixel
	size = image->sizeX * image->sizeY * 3;

	// Read the planes 
	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading the planes from %s\n", filename);
		return 0;
	}
	if (planes != 1) {
		printf("Planes from %s is not 1, but rather is : %u\n", filename, planes);
		return 0;
	}

	// Read the BPP
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading the bpp from : %s\n", filename);
		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24 but rather is : %u\n", filename, bpp);
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

	if ((i == fread(image->data, size, 1, file)) != 1) {
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

// Load the bitmaps and Convert them to textures
void loadGlTextures()
{
	Image *image1;								// Load a texture

	// Allocate memory for the texture
	image1 = (Image *)malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for the image\n");
		exit(0);								// Quit
	}
	
	const char * textureFile = "textures/Grass.bmp";			// Declare the filename

	if (!loadImage(textureFile, image1)) {
		exit(1);
	}

	glGenTextures(1, &texture[0]);				// Create the texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);	// 2D texture (x, y)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Scale linearly when img bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Scale linearly when img smaller than texture

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
	glLoadIdentity();						// Reset the view
	glTranslatef(0.f, 0.0f, -5.0f);		// Move 2.5 units right and 5 in
	glRotatef(rotCx, 1.0f, 0.0f, 0.0f);		// Rotate quad about x
	glRotatef(rotCy, 0.0f, 1.0f, 0.0f);		// Rotate quad about y
	glRotatef(rotCz, 0.0f, 0.0f, 1.0f);		// Rotate quad about z

	glBindTexture(GL_TEXTURE_2D, texture[0]);	// Select the texture to use

	// Draw a square 
	glBegin(GL_QUADS);					// Start drawing a quad

	 // Front Face (note that the texture's corners have to match the quad's corners)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
    
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
	
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    
    // Bottom Face       
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad

	glEnd();							// Finished drawing the quad

	rotCx += 1.5f;
	rotCy += 1.5f;
	rotCz += 1.5f;

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
