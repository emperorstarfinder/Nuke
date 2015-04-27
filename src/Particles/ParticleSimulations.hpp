/*
 *  Particle simulation file for setting up a particle
 *  groups to behave in some way.
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

#ifndef __GFX_PARTICLE_SIMULATIONS__
#define __GFX_PARTICLE_SIMULATIONS__

#include <math.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <stdio.h>

using namespace std;
using namespace glm;

namespace gfx {
	namespace part {
		/*
		 * ==================================================================
		 * Function		: BustSimulationSetup
		 *
		 * Description	: Sets up a group of particles to burst outwards from
		 *				  the origin with each particle having a different 
		 *				  color and velocity.
		 * ==================================================================
		 */
		template <class ParticleType>		
		void BurstSimulationSetup( ParticleType& particles ) {

			// Temp variables for color and velocity
			float velX = 0.001f, velY = 0.0f, velZ = 0.031f;
			float colR = 0.01f, colG = 0.01f, colB = 0.01f;
			float theta, mag = 0.03f;;

			// Total number of particles in system
			size_t numParticles = particles.MaxParticles();

			// initialize the size of the particles
			particles.initializerPolicy.shapePolicy.SetSize( vec3(0.1f, 0.1f, 0.1f) );

			for ( size_t i = 0; i < numParticles; i++ ) {
				// Determine the angle of the particle
				theta = 2 * 3.1415f / numParticles * i;

				// Get X and y using theta
				velX = mag * cos( theta );
				velY = sqrt( ( mag * mag ) - ( velX * velX ) );

				// Swap velocites for second half of array
				if ( i > (numParticles / 2 ) ) {
					velX *= -1.f;
					velY *= -1.f;
					velZ *= -1.f;
				}	

				// Determine colors
				if		( i <  ( numParticles / 3 ) )        {
					colB = 0.f; colG = 0.f;
				   	colR += ( 1.f / ( numParticles / 3 ) );		// Change redness
				} else if ( i <  ( numParticles / 3 * 2 ) )  {
					colR = 0.f; colB = 0.f;						// Change greenness
					colG += ( 1.f / ( numParticles / 3 ) );
				} else if ( i >= ( numParticles / 3 * 2 ) )	 {
					colR = 0.f; colG = 0.f;						// Change blueness
					colB += ( 1.f / ( numParticles / 3 ) );
				}

				particles.initializerPolicy.velocityPolicy.SetVelocity( vec3( velX, velY, velZ ) );		// Set particle velocity
				particles.initializerPolicy.colorPolicy.SetColor( vec4( colR, colG, colB, 1.0f ) );		// Set the color

				// Emit the particle at the x,y origin but into the screen
				particles.Emit( 1, vec3( 0.f, 0.f, -22.f ) ); 
			}

			// Load a texture for the particles 
			particles.AddTextures( {"textures/Cube.bmp"} );

			// Build the shapes for the particles
			particles.BuildShapes();
		};
	}		// End namespace part
}			// End namespace gfx

#endif		// __GFX_PARTICLE_SIMULATIONS__
