/*
 *  Movement Policy.
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

#ifndef	__GFX_PARTICLE_MOVEMENT_POLICY__
#define	__GFX_PARTICLE_MOVEMENT_POLICY__

#include <glm/vec3.hpp>
#include "../../Time/Time.hpp"

using namespace std;
using namespace glm;

namespace gfx {
	namespace part {
		/*
		 * =================================================================
		 * Class		: gfxMoveAction		
		 *
		 * Description	: Defines how the particle moves when updated.
		 * =================================================================
		 */
		template <class ParticleType>
		class gfxMoveAction
		{
			public:
				/*
				 * =========================================================
				 * Function		: PrepareAction 
				 *
				 * Description	: Performs any necessary setup required for
				 *				  the movement action
				 * =========================================================
				 */
				inline void PrepareAction() throw() {}

				/*
				 * =========================================================
				 * Function		: Operator () 
				 *
				 * Definition	: Updates the position of a particle by 
				 *				  using the velocity and time change.
				 *
				 * Params		: particle	: The particle which must be 
				 *							  moved.
				 * =========================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.pos += particle.velocity *
									vec3( gfx::time::DeltaTimef() );		
				}
		};	
	}			// End namespace part
}				// End namespace gfx

#endif			// __GFX_PARTICLE_MOVEMENT_POLICY__