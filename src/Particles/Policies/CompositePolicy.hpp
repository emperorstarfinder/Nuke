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

#ifndef __GFX_PARTICLE_COMPOSITE_POLICY__
#define __GFX_PARTICLE_COMPOSITE_POLICY__

using namespace std;

namespace gfx {
	namespace part {
		/*
		 * ================================================================
		 * Class		: gfxCompositePolicy
		 *
		 * Description	: Composes a policy from two (for now) already 
		 *				  defined policies.
		 * ================================================================
		 */
		template <class ParticleType, class Policy1, class Policy2>
		class gfxCompositePolicy : public Policy1, public Policy2
		{
			public:
				/*
				 * ========================================================
				 * Function		: PrepareAction 
				 *
				 * Description	: Calls the PrepareAction functions of 
				 *				  the composition policies.
				 * ========================================================
				 */
				inline void PrepareAction() throw() {
					Policy1::PrepareAction();
					Policy2::PrepareAction();
				}

				/*
				 * ========================================================
				 * Function		: Operator ()
				 *
				 * Description	: Invoke the Operator () functions of the
				 *				  composition policies on the particle.
				 *
				 * Params		: particle	: The particle on which to act
				 * ========================================================
				 */
				inline void operator() ( const ParticleType& particle) const throw() {
					Policy1::operator()(particle);
					Policy2::operator()(particle);
				}
		}
	}			// End namespace part
}				// End namespace gfx

#endif			// __GFX__PARTICLE_COMPOSITE_POLICY__
