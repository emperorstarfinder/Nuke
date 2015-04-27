/*
 *  Defines various gfxParticles types.
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

#ifndef __GFX__PARTICLE_TYPEDEFS__
#define __GFX__PARTICLE_TYPEDEFS__

#include "Policies/CompletePolicy.hpp"	
#include "Policies/CompositePolicy.hpp"		// For composing policies

using namespace gfx::part;

namespace gfx {
	namespace part {
		/*
		 * =============================================================
		 * Typedef		: ParticleInitializer
		 *
		 * Description	: Defines a general particle initializer for a 
		 *				  simple particle
		 * =============================================================
		 */
		typedef gfxCompletePolicy< gfxParticle							,	// Particle type
								   gfxLifeInitializer<gfxParticle>		,	// Life initializer
								   gfxColorInitializer<gfxParticle>		,	// Color initializer
								   gfxShapeInitializer<gfxParticle>		,	// Shape initializer
								   gfxTextureInitializer<gfxParticle>	,	// Texture initializer
								   gfxVelocityInitializer<gfxParticle>	,	// Velocity initializer
								   gfxNullPolicy<gfxParticle>			>	// Movement initializer
								ParticleInitializer;

	
		/*
		 * ============================================================
		 * Typedef		: ParticleAction 
		 *
		 * Description	: Defines a general particle action for a simple
		 *				  particle.
		 * ============================================================
		 */
		typedef gfxCompletePolicy< gfxParticle							,	// Particle type
								   gfxLifeAction<gfxParticle>			,	// Life action
								   gfxNullPolicy<gfxParticle>			,	// Color action
								   gfxNullPolicy<gfxParticle>			,	// Shape action
								   gfxNullPolicy<gfxParticle>			,	// Texture action
								   gfxNullPolicy<gfxParticle>			,	// Velocity action
								   gfxMoveAction<gfxParticle>			>   // Movement action
								ParticleAction;	
	}	// End namespace part
}		// End namepsace gfx

#endif	// __GFX_PARTICLE_TYPEDEFS__
