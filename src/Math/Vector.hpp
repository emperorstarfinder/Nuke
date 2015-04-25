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

#ifndef __GFX_MATHS_VECTOR__
#define __GFX_MATHS_VECTOR__

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace glm;

namespace gfx {
	namespace math {
		/*
		 * ===========================================================
		 * Function		: Vec3equal 
		 *
		 * Description	: Checks that two GLM vec3 vectors are equal,
		 *				  returns if the vectors are equal.
		 *
		 * Params		: left		: Input vector 1.
		 *				: right		: Input vector 2.
		 * ============================================================
		 */
		constexpr bool Vec3equal( const vec3& left, const vec3& right )  {
			// NOTE : Need to define function as const in C++14
			return (  (left.x == right.x) && 
					  (left.y == right.y) && 
					  (left.z == right.z)  ); 
		}

		/*
		 * ============================================================
		 * Function		: Vec4equal 
		 *
		 * Description	: Checks that two GLM vec4 vectors are equal,
		 *				  returns the boolean result.
		 *
		 * Params		: left		: Input vector 1
		 *				: right		: Input vector 2
		 * ============================================================
		 */
		constexpr bool Vec4equal(const vec4& left, const vec4& right) {
			// NOTE : Need to define function as const in C++14
			return (  (left.x == right.x) && (left.y == right.y) &&
					  (left.z == right.z) && (left.w == right.w)  );
		}
	}		// End namespace math
}			// End namespace gfx

#endif		// __NUKR_MATHS_VECTOR__

	
