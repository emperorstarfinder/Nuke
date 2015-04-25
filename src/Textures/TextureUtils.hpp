/*
 *  Utility functions for the texture loading.
 *
 *  Copyright (C) 2015 Rob Clucas robclu1818@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT AN_size.y WARRANTY; without even the implied warranty of
 *  MERCHANTABILIT_size.y or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  _size.you should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation,
 *	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __GFX_TEXTURE_UTILS__
#define __GFX_TEXTURE_UTILS__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef unsigned short ushort;

namespace gfx {
	namespace tex {	
		/*
		 * ================================================================
		 * Function		: Getint
		 *
		 * Description	: Gets a 32 bit integer from a file, returns the 
		 *				  integer.
		 *
		 * Params		: fp	: A FILE pointer to the file from which the 
		 *						  integer must be taken.
		 * ================================================================
		 */
		inline uint GetInt(FILE* fp) {
			int c, c1, c2, c3;

			// Get 4 bytes
			c  = getc(fp); c1 = getc(fp);
			c2 = getc(fp); c3 = getc(fp);

			// Return the result
			return (static_cast<uint>(c) + 
					(static_cast<uint>(c1) << 8) +
				    (static_cast<uint>(c2) << 16) + 
					(static_cast<uint>(c3) << 24));
		}

		/*
		 * ==================================================================
		 * Function		: Getshort
		 *
		 * Description	: Gets a short (16 bit integer) from a file, returns
		 *			      the integer. Function to get a short from a file.
		 *
		 * Params		: fp	: A FILE pointer to the file from which the 
		 *						  integer must be taken.
		 * ==================================================================
	     */
		inline ushort GetShort(FILE* fp) {
			int c, c1;

			// Get 2 bytes
			c = getc(fp); c1 = getc(fp);

			// Return the result
			return (static_cast<uint>(c) + (static_cast<uint>(c1) << 8));
		}
	}		// End namespace tex
}			// End namespac gfx

#endif		// __GFX_TEXTURE_UTILS__
