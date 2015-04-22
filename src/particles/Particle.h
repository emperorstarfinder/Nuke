/*
 *  Particle class for Nuke.
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

#ifndef __NUKE_PARTICLE__
#define __NUKE_PARTICLE__

#include <GL/gl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../texture/texture.hpp"

using namespace std;
using namespace glm;			// For vectors
using namespace nuke::tex;		// For texture

namespace nuke {
	namespace part {
		/**
		 *  \struct Definition of a particle.
		 */
		struct Particle 
		{
			vec3 pos;			/**< Particle position. */
			vec3 velocity;		/**< Particle velocity. */
			vec4 color;			/**< Particle color.	*/
			vec3 size;			/**< Particle size.		*/
			float life;			/**< Particle life.		*/
			uint texture;	    /**< Particle texture index in particles textures vector. */
		};
	}	// End namespace part
}		// End namespace nuke

#endif // __NUKE_PARTICLE__