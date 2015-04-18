/*
 *  Draw functions for the Nuke renderer.
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

#ifndef __NUKE_RENDERER_DRAW__
#define __NUKE_RENDERER_DRAW__

#include "buildlists.hpp"

namespace nuke {
	namespace rend {
	/** Function that draws an OpenGL object to the screen.
	 * Currently this is slow and badly designed but I'm going for functional
	 * over performance to start.
	 */
	template <class DrawableObject>
	GLvoid Draw(const DrawableObject& object) throw() 
	{	
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT);

		// Now the texture to be used needs to be set. Each drawable objects
		// should have a texture
		// glBindTexture(X,Y)

	}

	}	// End namespace rend
}		// End namespace nuke
