/*
 *  Functions that are used to operate on GLM vectors.
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

#ifndef __NUKE_MATHS_VECTOR__
#define __NUKE_MATHS_VECTOR__

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace glm;

namespace nuke {
	namespace maths {
		/** Function to check for equivalence of two 3 dimensional vectors
		 * 
		 * @param left The one vector to check for equivalence with.
		 * @param right The other vector to check for equivalence with.
		 * @return If the vecctors are equivalent.
		 */
		constexpr bool vec3equal(const vec3& left, const vec3& right)  // const for c++14
		{
			return (  (left.x == right.x) && 
					  (left.y == right.y) && 
					  (left.z == right.z)  ); 
		}

		constexpr bool vec4equal(const vec4& left, const vec4& right) // const for c++14 
		{
			return (  (left.x == right.x) && (left.y == right.y) &&
					  (left.z == right.z) && (left.w == right.w)  );
		}
	}		// End namespace maths
}			// End namespace nuke

#endif		// __NUKR_MATHS_VECTOR__

	
