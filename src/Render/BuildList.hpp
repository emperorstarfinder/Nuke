/*
 *  Build lists for file for bulding custom shapes. Defines simple 
 *  shapes which are compiled by OpenGL so that theey run faster.
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

#ifndef __GFX_RENDERER_BUILDLISTS__
#define __GFX_RENDERER_BUILDLISTS__

#include <GL/gl.h>				// OpenGL
#include "../Shape/Shape.hpp"	// Shapes

using namespace glm;
using namespace gfx::shape;		// For DrawableShape

namespace gfx {
	namespace rend {
		/*
		 * =================================================================================
		 * Function			: CubeList
		 *
		 * Description		: Compile a cube of a specific sizze for OpenGL.
		 *
		 * Params			: _size			: A 3 dimensional vector where each
		 *									  dimension is the length of the size of the 
		 *									  cube.
		 *					: listPointer	: A pointer to the GLuint handle for the cube.
		 * ==================================================================================
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

		/*
		 * =====================================================================================================
		 * Function			: BuildList 
		 *
		 * Description		: Builds a list for the given shape.
		 *
		 * Params			: shape			: The shape that the list should build.
		 *					: listPointer	: A pointer to the GLuint handle for the built list.
		 * ====================================================================================================
		 */
		GLvoid BuildList(DrawableShapes shape, const vec3& size, GLuint* listpointer)
		{
			// Check which build list function must be called
			switch (shape) {
				case CUBE:
					CubeList(size, listpointer);
					break;

				default:
					break;
			}
		}	
	}	// End namespace rend
}		// End namespace gfx

#endif	// __GFX_RENDERER_BUILDLISTS__
