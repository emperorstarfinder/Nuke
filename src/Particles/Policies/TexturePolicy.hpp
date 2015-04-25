/*
 *  Texture Policy.
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

#ifndef __GFX_PARTICLE_TEXTURE_POLICY__
#define __GFX_PARTICLE_TEXTURE_POLICY__

using namespace std;

namespace gfx {
	namespace part {
		/*
		 * ==================================================================
		 * Class		: gfxTextureInitializer
		 *
		 * Description	: Texture initializer policy that sets the texture
		 *				  to use for a particle.
		 * ==================================================================
		 */
		template <class ParticleType>
		class gfxTextureInitializer
		{
			public:
				unsigned int texture;	// Handle for the texture to use
			public:
				/*
				 * =========================================================
				 * Function		: gfxTextureInitializer 
				 *
				 * Description	 : Construct the texture initializer and 
				 *				   sets the first texture as the texture
				 *				   to use.
				 * =========================================================
				 */
				explicit gfxTextureInitializer() throw() : 
					texture( 0 ) {}

				/*
				 * =========================================================
				 * Function		: SetTexture 
				 *
				 * Description	: Sets the texture handle used for 
				 *				  rendering a texture.
				 *
				 * Params		: handle	: The handle to use when 
				 *							  initializing a texture
				 * =========================================================
				 */
				inline void SetTexture( const unsigned int index ) throw() {
					texture = index;
				}

				/*
				 * =========================================================
				 * Function		: Operator ()
				 *
				 * Description	: Sets the texture handle of the particle to 
				 *				  that defined in this class.
				 *
				 * Params		: particle	: The particle for which the 
				 *							  texture handle will be set.
				 * =========================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.texture = texture;
				}
		};
	}			// End namespace part
}				// End namespcea gfx

#endif			//	__GFX_PARTICLE_TEXTURE_POLICY__
