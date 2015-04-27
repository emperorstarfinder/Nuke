/*
 *  Shape Policy.
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

#ifndef __GFX_PARTICLE_SHAPE_POLICY__
#define __GFX_PARTICLE_SHAPE_POLICY__

#include <glm/vec3.hpp>
#include "../../Shape/Shape.hpp"

using namespace std;
using namespace glm;
using namespace gfx::shape;

namespace gfx {
	namespace part {
		/*
		 * =================================================================
		 * Class		: gfxShapeInitializer 
		 *
		 * Description	: Initializes a particle's shape using the gfxShape 
		 *				  class.
		 * =================================================================
		 */
		template <class ParticleType>
		class gfxShapeInitializer
		{
			public:
				vec3		   size;		// Size (x,y,z dimension) of the particle
				DrawableShapes type;		// Type of shape from DrawableShapes
			public:
				/*
				 * ============================================================
				 * Function		: gfxShapeInitializer
				 *
				 * Description	: Constructs the shape initializer using a cube
				 *				  as the defualt shape, with unit volume
				 * ============================================================
				 */
				explicit gfxShapeInitializer() throw() : 
					type( DrawableShapes::CUBE ), size( vec3( 1.f, 1.f, 1.f ) ) {}

				/*
				 * ============================================================
				 * Function		: SetSize
				 *
				 * Description	: Sets the size of a particle. The size is a 3D
				 *				  vector giving the length of each dimension.
				 *
				 * Params		: _size		: The size of each dimension of the 
				 *							  particle.
				 * =============================================================
				 */
				inline void SetSize( const vec3& _size ) throw() {
					size = _size;
				}

				/*
				 * =============================================================
				 * Function		: SetType
				 *
				 * Description	: Sets the type of shape the particle will be,
				 *				  where the options are from the 
				 *				  DrawableShapes enum in the gfx::shape
				 *				  namespace.
				 *
				 * Params		: _type		: The DrawableShapes type.
				 * =============================================================
				 */
				inline void SetType( const DrawableShapes& _type ) throw() {
					type = _type;
				}

				/*
				 * =============================================================
				 * Function		: Operator ()
				 *
				 * Description	: Sets the gfxShape type and size for a particle.
				 *
				 * Params		: particle	: The particle for which the 
				 *							  gfxShape will be set.
				 * =============================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.shape = gfxShape( type, size );
				}
		};
	}			// End namespace part
}				// End namespace gfx

#endif			// __GFX_PARTICLE_SHAPE_POLICY__

