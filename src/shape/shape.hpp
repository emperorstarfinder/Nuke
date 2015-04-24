/*
 *  'Shape' class for Nuke. This does not define a shape, rather it defines a
 *  key that can be used to get a unique build list that defines the shape,
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

#ifndef __NUKE_SHAPE_SHAPE__
#define __NUKE_SHAPE_SHAPE__

#include <unordered_map>			// For hash
#include "../maths/vector.hpp"

using namespace std;		
using namespace nuke::maths;		// For vec3equal()

namespace nuke {
	namespace shape {

		/** Enum providing all drawable shapes. */
		enum DrawableShape {
			NONE = 0,
			CUBE = 1
		};

		/** 
		 * \class Shape class that identifies a shape using its name and size 
		 *  to calculate its hash value.
		 */
		class Shape 
		{
			public:
				DrawableShape type;		/**< Name of the shape. */
				vec3 size;				/**< Size of the shape.	*/
			public:
				/** Default constructor. */
				Shape() throw() : type(DrawableShape::CUBE), size(vec3(1.0f, 1.0f, 1.0f)) {}

				/** Constructor for the shape class.
				 *
				 * @param _type The type of the shape.
				 * @param _size The size of the shape.
				 */
				explicit Shape(DrawableShape _type, vec3 _size) throw() : type(_type), size(_size) {}

				/** Equality operator.
				 *
				 * @param shape The shape with which equality must be checked.
				 * @return If the shapes are equal.
				 */
				inline bool operator== (const Shape& shape) const throw() {
					return ((shape.type == type) && (vec3equal(shape.size, size)));
				}

				/** Assignment operator.
				 *
				 * @param shape The shape with which to make the shape equal to.
				 */
				inline void operator= (const Shape& shape) throw() {
					type = shape.type;
					size = shape.size;
				}
		};

		/** 
		 * \class Class used to hash a shape. 
		 */
		class ShapeHash
		{
			public:
				inline size_t operator() (const Shape& shape) const throw() {
					return (   hash<int>()(shape.type)   ^ 
							   hash<float>()(shape.size.x) ^
							   hash<float>()(shape.size.y) ^
							   hash<float>()(shape.size.z)   );
				}
		};
	}		// End namespace shape 
}			// End namespace nuke

#endif		// __NUKE_SHAPE_SHAPE__