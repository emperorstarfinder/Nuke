/*
 *  'Shape' class. This does not define a shape, rather it defines a
 *  key that can be used to get a unique build list that creates the shape,
 *  which is identified by its name and size. 
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

#ifndef __GFX_SHAPE_SHAPE__
#define __GFX_SHAPE_SHAPE__

#include <unordered_map>			// For hash
#include "../Math/Vector.hpp"		// For Vec3equal

using namespace std;		
using namespace gfx::math;			

namespace gfx {
	namespace shape {
		/*
		 * ==================================================================
		 * Enum			: DrawableShapes
		 *
		 * Description	: Provides a list of all drawable shapes.
		 * ==================================================================
		*/
		enum DrawableShapes {
			NONE = 0,
			CUBE = 1
		};

		/*
		 * ==================================================================
		 * Class		: Shape 
		 *
		 * Description	: Defines a shape by its name and size. A hash value 
		 *				  is calculated using these and uniquely identifies 
		 *				  the shape. The shape name can be used to call a 
		 *				  build list to draw the shape, and the unique hash 
		 *				  can be used as a key to store the shape type.
		 * ===================================================================
		 */
		class gfxShape 
		{
			public:
				DrawableShapes type;		// Name of the shape
				vec3		   size;			// Size of the shape.	
			public:
				/*
				 * ===========================================================
				 * Function		: Shape
				 *
				 * Description	: Default constructor for the gfxShape class. 
				 *				  Sets the shape to be a cube with unit volume.
				 * ===========================================================
				 */
				explicit gfxShape() throw() : type( DrawableShapes::CUBE ),  
											  size( vec3( 1.0f, 1.0f, 1.0f ) ) {}

				/*
				 * ============================================================
				 * Function		: Shape 
				 *
				 * Description	: Constructor for gfxShape class that uses a
				 *				  shape type and size for initialization.
				 *
				 * Params		: _type		: The type for the shape, from the
				 *							  DrawableShapes enum.
				 *				: _size		: The 3D size of the shape.
				 * =============================================================
				 */
				explicit gfxShape( DrawableShapes _type, vec3 _size ) throw() : 
					type( _type ), size( _size ) {}

				/*
				 * =============================================================
				 * Function		: Operator ==
				 *
				 * Description	: Determines if two shapes are equivalent.
				 *
				 * Params		: shape		: The other shapes for which equality 
				 *							  must be checked.
				 * =============================================================
				 */
				inline bool operator== ( const gfxShape& shape ) const throw() {
					return ( (shape.type == type) && 
							 ( Vec3equal( shape.size, size ) ) );
				}

				/*
				 * ==============================================================
				 * Function		: Operator = 
				 *
				 * Description	: Assigns one gfxShape to another gfxShape.
				 *
				 * Params		: shape		: The shape for which this shape
				 *							  must be made equivalent.
				 * ==============================================================
				 */
				inline void operator= ( const gfxShape& shape ) throw() {
					type = shape.type;
					size = shape.size;
				}
		};

		/*
		 * ======================================================================
		 * Class		: ShapeHash
		 *
		 * Description	: Class used to compute the hash of a gfxShape
		 * ======================================================================
		 */
		class gfxShapeHash
		{
			public:
				/*
				 * ==============================================================
				 * Function		: Operator ()
				 *
				 * Description	: Computes the hash of a gfxShape by XORing the 
				 *				  type with the size of each dimension.
				 *
				 * Params		: shape		: The gfxShape for which the has
				 *							  must be computed.
				 * ==============================================================
				 */
				inline size_t operator() ( const gfxShape& shape ) const throw() {
					return (   hash<int>()( shape.type )     ^ 
							   hash<float>()( shape.size.x ) ^
							   hash<float>()( shape.size.y ) ^
							   hash<float>()( shape.size.z )   );
				}
		};
	}		// End namespace shape 
}			// End namespace gfx

#endif		// __GFX_SHAPE_SHAPE__
