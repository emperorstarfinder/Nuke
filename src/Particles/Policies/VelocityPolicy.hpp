/*
 *  Velocity Policy.
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

#ifndef __GFX_PARTICLE_VELOCITY_POLICY__
#define __GFX_PARTICLE_VELOCITY_POLICY__

#include <glm/vec3.hpp>
#include "../../Time/Time.hpp"

using namespace std;
using namespace glm;

namespace gfx {
	namespace part {
		/*
		 * =================================================================
		 * Class		: gfxVelocityInitializer
		 *
		 * Description	: Initializes the velocity of a particle.
		 * =================================================================
		 */
		template <class ParticleType>
		class gfxVelocityInitializer
		{
			public:
				vec3 velocity;		// Velocity to initialize particle with
			public:
				/*
				 * ========================================================
				 * Function		: gfxVelocityInitializer 
				 *
				 * Description	: Constructs the velocity initializer with 
				 *				  no initial velocity
				 * ========================================================
				 */
				explicit gfxVelocityInitializer() throw() : 
					velocity( vec3( 0.f ) ) {}

				/*
				 * ========================================================
				 * Function		: SetVelocity 
				 *
				 * Description	: Sets the velocity that a particle will
				 *				  be initialized with.
				 *
				 * Params		: _velocity		: The velocity to initialize
				 *								  the particle with.
				 * =========================================================
				 */
				inline void SetVelocity( const vec3& _velocity ) throw() {
					velocity = _velocity;
				}

				/*
				 * =========================================================
				 * Function		: Operator ()
				 *
				 * Description	: Sets the velocity of the particle to the 
				 *				  velocity defined in this class.
				 *
				 * Params	: particle	: The particle for which the 
				 *						: velocity will be set.
				 * =========================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.velocity = velocity;
				}
		};

		/*
		 * ==================================================================
		 * Class		: gfxVelocityAction
		 *
		 * Description		: Action policy for updating the velocity of a 
		 *					  particle using the velocity and change in time 
		 * ==================================================================
		 */
		template <class ParticleType>
		class gfxVelocityAction
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
				 * Definition	: Updates the velocity of a particle using 
				 *				  its acceleration and the time change.
				 *
				 * Params		: particle	: The particle which must have
				 *							  its velocity updated.
				 * =========================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.velocity += particle.acceleration *
										 vec3( gfx::time::DeltaTimef() );		
				}	
			
	}			// End namespace part
}				// End namespace gfx

#endif			// __GFX_PARTICLE_VELOCITY_POLICY__
