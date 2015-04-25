/*
 *  Color Policy
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

#ifndef __GFX_PARTICLE_COLOR_POLICY__
#define __GFX_PARTICLE_COLOR_POLICY__

namespace gfx {
	namespace part {
		/*
		 * ==========================================================
		 * Class		: gfxColorInitializer
		 *
		 * Description	: Initializes the color of a particle.
		 * ==========================================================
		 */
		template <class ParticleType>
		class gfxColorInitializer
		{
			public: 
				vec4 color;				// Color to initialize particle with
			public:
			   /* 
				* ===================================================
				* Function		: gfxColorInitializer
				*
				* Description	: Constructs the color initializer, 
				*				  setting the color to white.
				* ====================================================
				*/
				explicit gfxColorInitializer() throw() :
				   	color( 1.f, 1.f, 1.f, 1.f ) {}

				/*
				 * ===================================================
				 * Function		: SetColor	
				 *
				 * Description	: Sets the color to initialize a 
				 *				  particle with.
				 *
				 * Params		: _color	: The color (RGBA) to use
				 *							  to initialize particles 
				 * ===================================================
				 */
				inline void SetColor( const vec4& _color ) throw() {
					color = _color;
				}

				/*
				 * ====================================================
				 * Function		: Operator ()
				 *
				 * Description	: Applies the color to a particle.
				 *
				 * Params		: particle	: The particle for which 
				 *							  the color will be set.
				 * ====================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.color = color;
				}
		};	
	}			// End namespace part
}				// End namespace gfx

#endif			// __GFX_PARTICLE_COLOR_POLICY__
