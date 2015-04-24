/*
 *  Visual tests for the particles class, testing if various features 
 *  behave correctly.
 *
 *  Copyright (C) 2015 Rob Clucas robclu1818@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation,
 *	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>			// Allows standard io
#include <stdlib.h>			// Needed for malloc and free
#include <unistd.h>			// Sw that the sleep function can be called
#include <GL/glut.h>		// Header for the GLUT library
#include <GL/gl.h>			// Header for the OpenGL32 library
#include <GL/glut.h>
#include "Particles.hpp"

#define ESCAPE 27			// Macro for ESC key

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const size_t NUM_PARTICLES = 1;

int window;					// Handle for the window

using namespace nuke::part;

// Define the particles to use for the visual test
// Initially the particles will be drawn without a texture
Particles<NUM_PARTICLES, Particle, ParticleInitializer, ParticleAction> particles;

// Emits the particles and sets their velocity
void particleConfig()
{
	particles.initializerPolicy.velocityPolicy.SetVelocity(vec3(0.2f, 0.3f, 0.f));	// Set their velocity
	particles.initializerPolicy.shapePolicy.SetSize(vec3(1.f, 1.f, 1.f));			// Set their size
	particles.initializerPolicy.shapePolicy.SetType(DrawableShape::CUBE);			// Set thir shape

	// Emit NUM_PARTICLES at the origin
	particles.Emit(NUM_PARTICLES, vec3(0.f, 0.f, 0.f)); 

	// Build the shapes for the particles
	particles.BuildShapes();
}

// Function to draw the scene. 
// The arguments need to be void
void drawScene()
{
	// First draw the particles
	particles.Draw();

	// Then update them
	particles.Update();
}

// Initialize OpenGL 
void initGL(int width, int height)
{
	// Can set the texture here if we want
	// Need to call the buildlist here
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
	
// Resize the screen if the size is changed
void resizeScene(int width, int height)
{
	// Prevent division by 0
	if (height = 0) height = 1;

	// Reset current viewport and perspective
	glViewport(0, 0, width, height);	

	glMatrixMode(GL_PROJECTION);		// Tell GL we want to operate on perspective
	glLoadIdentity();					// Reset the perspective matrix

	// Calculate the aspect ratio of the window
	gluPerspective(45.f, static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1f, 100.f);
	glMatrixMode(GL_MODELVIEW);			// Back to modelview mode
}   

int main(int argc, char** argv) 
{
	// Set the particle properties
	particleConfig();

	// Init GLUT state
	glutInit(&argc, argv);

	// Select the display mode for GLUT
	// [RGBA | Double Buffer | Alpha Channel Enabled | Depth Buffer ]
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	// Initialize the window size
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Make the top left of the screen (0, 0)
	glutInitWindowPosition(0, 0);

	// Open a window
	window = glutCreateWindow("Particle Visual Tests");

	// Initialize the OpenGL drawing function 
	glutDisplayFunc(&drawScene);

	// Make the window fullscreen 
	glutFullScreen();

	// Draw the scene regardless of the presence of events
	glutIdleFunc(&drawScene);

	// Initialize the function to resize the screen
	glutReshapeFunc(&resizeScene);

	// Register the keyboard checking function 
	glutKeyboardFunc(&keyPressed);

	// Check for special keys
	// glutSpecialFunc(&<Special Function Name>);
	
	// Initialize OpenGL
	initGL(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Start the main loop 
	glutMainLoop();

	return 1;
}

