/*
 *  Stores and loads textures.
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

#ifndef __NUKE_TEXTURE__
#define __NUKE_TEXTURE__

#include <sstream>
#include <string>
#include <iostream>

typedef unsigned long ulong;

using namespace std;

// Compute hash of string at compile time - thanks to the internet for helping 
constexpr unsigned int strtoui(const char* str, int hash = 0) 
{
	return !str[hash] ? 5381 : (strtoui(str, hash + 1) * 33) ^ str[hash];
}

namespace nuke {
	namespace tex {
		/** Class for storing a texture from various file formats. */
		class Texture 
		{
			private :
				ulong sizeX;		/**< The size of the texture in the x dimension. */
				ulong sizeY;		/**< The size of the texture in the y dirextion. */
				char* data;			/**< The texture data. */
			
			public :
				/** Load a texture from a file.
				 *
				 * @param file The file from which the texture must be loaded in
				 * the format:
				 *				<Name><Size>.<Type>
				 * where :
				 *			Name = Name of the texture
				 *			Size = Size in 3 digit format
				 *			Type = The type of file
				 */
				Texture(const string file) throw();

			private :
				/** Function to load a texture from a bitmap.
				 *
				 * @param file The file to get the texture from.
				 * @param _sizeX The size of the image in the x direction.
				 * @param _sizeY The size of the image in the y direction.
				 */
				void loadBitmap(const string file, const ulong _sizeX, const ulong _sizeY) throw();
		};

		Texture::Texture(const string file) throw() {
			string filetype, tsize;						// Temp strings
			ulong texsize;								// Size of the texture

			size_t dotpos = file.find(".");				// Find position of '.'
			tsize = file.substr(dotpos - 3, dotpos);	// Size of the texture
			filetype = file.substr(dotpos + 1);			// Everything after the .

			// Convert size to a number
			istringstream istream(tsize);				// Put into string stream
			istream >> texsize;							// Convert to number

			// Use the relevant function for the filetype (using the compile
			// time hash of the string)
			switch (strtoui(filetype.c_str())) {
				case strtoui("bmp"):
					loadBitmap(file, texsize, texsize);
					break;

				default:
					break;

				}
		}

		void Texture::loadBitmap(const string file, const ulong _sizeX, const ulong _sizeY) throw() {
		}

	}	// End namspace tex
}		// End namespace nuke

#endif	// __NUKE_TEXTURE__
