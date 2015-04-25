/*
 *  Complete Policy.
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

#ifndef __GFX_PARTICLE_COMPLETE_POLICY__
#define __GFX_PARTICLE_COMPLETE_POLICY__

#include "Particle.h"
#include "ColorPolicy.hpp"
#include "LifePolicy.hpp"
#include "MovementPolicy.hpp"
#include "NullPolicy.hpp"
#include "ShapePolicy.hpp"
#include "TexturePolicy.hpp"
#include "VelocityPolicy.hpp"
#include <cstdlib>							// rand()
#include <math.h>							// fmod()

namespace gfx {
	namespace part {
		/*
		 * ====================================================================
		 * Class		: gfxCompletePolicy
		 *
		 * Description	: Policy class that wraps all other policies into a 
		 *				  single policty.
		 *
		 * Additional	: To add functionality to the particles simply make  a
		 *				  new policy and add it here in the same format.
		 * =====================================================================
		 */
		template <class ParticleType, class LifePolicy	 , class ColorPolicy	,
				  class ShapePolicy	, class TexturePolicy, class VelocityPolicy	, 
				  class MovementPolicy>
		class CompletePolicy
		{
			public :
				gfxLifePolicy		lifePolicy;			// Life policy for the particles
				gfxColorPolicy		colorPolicy;		// Color policy for the particles	 
				gfxShapePolicy		shapePolicy;		// Size policy for the particles
				gfxTexturePolicy	texturePolicy;		// Texture policy for the particles
				gfxVelocityPolicy	velocityPolicy;		// Velocity policy for the particles
				gfxMovementPolicy	movementPolicy;		// Movement policy for the particles

				/*
				 * ================================================================
				 * Function		: PrepareAction 
				 *
				 * Description	: Prepares the action of policies by calling each 
				 *				  one's preparation function.
				 * ================================================================
				 */
				inline void PrepareAction() throw() {
					lifePolicy.PrepareAction();
					colorPolicy.PrepareAction();
					shapePolicy.PrepareAction();
					texturePolicy.PrepareAction();
					velocityPolicy.PrepareAction();
					movementPolicy.PrepareAction();
				}

				/*
				 * ================================================================
				 * Function		: Operator ()
				 *
				 * Description	: Function that invokes the operation of the
				 *				  policies on the particle.
				 *
				 * Params		: particle	: The particle on which the 
				 *							  gfxComplete policy will act.
				 * ================================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					lifePolicy( particle );				// Give particle life
					colorPolicy( particle );			// Give the particle color
					shapePolicy( particle );			// Give the particle size
					texturePolicy( particle );			// Give the particle a texture to use
					velocityPolicy( particle );			// Perform velocity related actions
					movementPolicy( particle );			// Move the particle
				}
	}			// End namespace gfx
}				// End namespace part

#endif			// __GFX_PARTICLE_COMPLETE_POLICY__
