/*
 *  Build lists for rendering class for Nuke. Defines simple shapes which
 *  are compiled by OpenGL so that theey run faster.
 *
 *  Copyright (C) 2015 Rob Clucas robclu1818@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT AN_size.y WARRANTY; without even the implied warranty of
 *  MERCHANTABILIT_size.y or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  _size.you should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation,
 *	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __NUKE_RENDERER_BUILDLISTS__
#define __NUKE_RENDERER_BUILDLISTS__

#include <GL/gl.h>				// OpenGL
#include "../shape/shape.hpp"	// Shapes

using namespace glm;
using namespace nuke::shape;	// For DrawableShape

namespace nuke {
	namespace rend {
		/** Function to generate a cube of a specific size that is compiled by
		 * OpenGL so that it's faster.
		 *
		 * @param _size The size of each side of the cube.
		 * @aram  _listPointer The integer used to create the list
		 */
		GLvoid CubeList(const vec3& _size, GLuint* listPointer)
		{
			*listPointer = glGenLists(1);			// Only need one list for a cube
			glNewList(*listPointer, GL_COMPILE);	// Store the list at listPointers location & compile

			glBegin(GL_QUADS);
			// Bottom face
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(-0.5f * _size.x, -0.5f * _size.y, -0.5f * _size.z);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f( 0.5f * _size.x, -0.5f * _size.y, -0.5f * _size.z);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f( 0.5f * _size.x, -0.5f * _size.y,  0.5f * _size.z);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(-0.5f * _size.x, -0.5f * _size.y,  0.5f * _size.z);	// Bottom Right Of The Texture and Quad
			// Front Face
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-0.5f * _size.x, -0.5f * _size.y,  0.5f * _size.z);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f( 0.5f * _size.x, -0.5f * _size.y,  0.5f * _size.z);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f( 0.5f * _size.x,  0.5f * _size.y,  0.5f * _size.z);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(-0.5f * _size.x,  0.5f * _size.y,  0.5f * _size.z);	// Top Left Of The Texture and Quad
			// Back Face
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(-0.5f * _size.x, -0.5f * _size.y, -0.5f * _size.z);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(-0.5f * _size.x,  0.5f * _size.y, -0.5f * _size.z);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f( 0.5f * _size.x,  0.5f * _size.y, -0.5f * _size.z);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f( 0.5f * _size.x, -0.5f * _size.y, -0.5f * _size.z);	// Bottom Left Of The Texture and Quad
			// Right face
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f( 0.5f * _size.x, -0.5f * _size.y, -0.5f * _size.z);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f( 0.5f * _size.x,  0.5f * _size.y, -0.5f * _size.z);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f( 0.5f * _size.x,  0.5f * _size.y,  0.5f * _size.z);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f( 0.5f * _size.x, -0.5f * _size.y,  0.5f * _size.z);	// Bottom Left Of The Texture and Quad
			// Left Face
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-0.5f * _size.x, -0.5f * _size.y, -0.5f * _size.z);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(-0.5f * _size.x, -0.5f * _size.y,  0.5f * _size.z);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(-0.5f * _size.x,  0.5f * _size.y,  0.5f * _size.z);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(-0.5f * _size.x,  0.5f * _size.y, -0.5f * _size.z);	// Top Left Of The Texture and Quad
			// Top face
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(-0.5f * _size.x, 0.5f * _size.y, -0.5f * _size.z);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(-0.5f * _size.x, 0.5f * _size.y,  0.5f * _size.z);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f( 0.5f * _size.x, 0.5f * _size.y,  0.5f * _size.z);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f( 0.5f * _size.x, 0.5f * _size.y, -0.5f * _size.z);	// Top Right Of The Texture and Quad
			
			glEnd();		// End drawing
			glEndList();	// End list
		}

		/** Function to determine which buildlist to create and then to call the
		 * relevant function. 
		 *
		 * @param _size The size of the object to build.
		 * @param listPointer A pointer to the build list.
		 */
		GLvoid BuildList(DrawableShape shape, const vec3& size, GLuint* listpointer)
		{
			// Check which shape list function must be called
			switch (shape) {
				case CUBE:
					CubeList(size, listpointer);
					break;

				default:
					break;
			}
		}	
	}	// End namespace rend
}		// End namespace nuke

#endif	// __NUKE_RENDERER_BUILDLISTS__
