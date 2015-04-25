/*
 *  Null Policy
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

#ifndef __GFX_PARTICLE_NULL_POLICY__
#define	__GFX_PARTICLE_NULL_POLICY__

using namespace std;

namespace gfx {
	namespace part {
		/*
		 * ================================================================
		 * Class		: gfxNullPolicy
		 *
		 * Description	: Null policy which does nothing. Compiler will 
		 *				 works out that this does nothing so it wont be 
		 *				 costly.
		 * ================================================================
		 */
		template <class ParticleType>
		class gfxNullPolicy
		{
			public:
				/*
				 * ========================================================
				 * Function		: PrepareAction 
				 *
				 * Description	: Null function which is used for the 
				 *				  to specify an action for a particle when
				 *				  no action is required for the policy.
				 * ========================================================
				 */
				inline void PrepareAction() throw() {}

				/*
				 * ========================================================
				 * Function		: Operator ()
				 *
				 * Description	: Null function which does nothing but make 
				 *				  the compiler happy when an 'empty' policy
				 *				  is required for a particle.
				 *
				 * Params		: particle	: The particle on which the 
				 *							  null policy acts.
				 * ========================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {}
		};
	