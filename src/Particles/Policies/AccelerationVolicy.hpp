/*
 *  Acceleration Policy.
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

#ifndef __GFX_PARTICLE_ACCELERATION_POLICY__
#define __GFX_PARTICLE_ACCELERATION_POLICY__

#include <glm/vec3.hpp>

using namespace std;
using namespace glm;

namespace gfx {
	namespace part {
		/*
		 * ==================================================================
		 * Class		: gfxAccelerationInitializer 
		 *
		 * Description	: Initializes the acceleration of a particle.
		 * ==================================================================
		 */
		template <class ParticleType>
		class gfxAccelerationInitializer
		{
			public:
				vec3 acceleration;		// Acceleration to intialize the particle with
			public:
				/*
				 * ===========================================================
				 * Function			: gfxAccelerationInitializer
				 *
				 * Description		: Constructs the acceleration initializer
				 *					  with only gravitational acceleration.
				 * ===========================================================
				 */
				explicit gfxAccelerationInitializer() throw() :
					acceleration( vec3( 0.0f, -9.81f, 0.0f ) ) {}

				/*
				 * ===========================================================
				 * Function			: SetAcceleration 
				 *
				 * Description		: Sets the acceleration that the particle
				 *					  will be initialized with.
				 *
				 * Params			: _accceleration	: The acceleration to
				 *										  initialize with.
				 * ============================================================
				 */
				inline void SetAcceleration( const vec3& _acceleration ) throw() {
					acceleration = _acceleration;
				}

				/*
				 * ============================================================
				 * Function			: Operator ()
				 *
				 * Description		: Sets the acceleration of a particle to the 
				 *					  class acceleration variable.
				 *
				 * Params			: particle	: The particle to set the 
				 *								  acceleration for.
				 * =============================================================
				 */
				inline void operator() ( const ParticleType& particle ) const throw() {
					particle.acceleration = acceleration;
				}
		};
	}			// End namespace part
}				// End namespace gfx

#endif			// __GFX_PARTICLE_ACCELERATION_POLICY
					