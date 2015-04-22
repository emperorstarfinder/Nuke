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
#include <iostream>
#include <string.h>
#include "textureutils.hpp"

typedef unsigned long ulong;
typedef unsigned short int ushorti;

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
				/** Default constructor for a texture that creates an empty
				 * texture. */
				explicit Texture() throw() : sizeX(0), sizeY(0), data(NULL) {}

				/** Load a texture from a file.
				 *
				 * @param filename The name of the file from which the texture must be loaded in
				 * the format:
				 *				<Name><Size>.<Type>
				 * where :
				 *			Name = Name of the texture
				 *			Size = Size in 3 digit format
				 *			Type = The type of file
				 */
				explicit Texture(const char* filename) throw();

				/** Descructor, frees memory on exit. */
				~Texture() throw() {
					if (data != NULL) {
						free(data);
						data = NULL;
					}
				}

				/** Returns the size of the texture's x dimension.
				 *
				 * @return The size of the x dimension of the texture.
				 */
				inline ulong SizeX() const { return sizeX; }

				/** Returns the size of the texture's y dimension.
				 *
				 * @return The size of the y dimension (height) of the texture.
				 */
				inline ulong SizeY() const { return sizeY; }

				/** Returns the texture data.
				 *
				 * @return A pointer to the texture data.
				 */
				inline char* Data() const { return data; }

			private :
				/** Function to load a texture from a bitmap.
				 *
				 * @param filename The name of the file to get the texture from.
				 * @return If the texture was loaded from the bitmap.
				 * */
				bool loadBitmap(const char* filename) throw();
		};

		// Constructor when given the texture filename
		Texture::Texture(const char* file) throw() {
		
			const char* filetype = strchr(file, '.');		// Get .<filetype>
			filetype++;										// Get rid of '.'

			// Use the relevant function for the filetype (using the compile
			// time hash of the string)
			switch (strtoui(filetype)) {
				
				// Bitmap file
				case strtoui("bmp"):
					if (!loadBitmap(file)) {
						cout << "ERROR T01 : Texture could not be loaded from bitmap file " << file << endl;
					}
					break;

				default:
					break;

				}
		}

		// Function that loads a texture from a bitmap
		// See here for more info on bitmaps :
		// http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt 
		bool Texture::loadBitmap(const char* filename) throw() {
			FILE *file;					// File handle
			ulong size;					// Image size (in bytes)
			ulong i;					// Counter
			ushorti planes;				// Number of planes in image (must be 1)
			ushorti bpp;				// Bits per pixel (must be 24)
			char temp;					// Temp col storage for bgr->rgb conversion
	
			// Make sure that the file exists
			if ((file = fopen(filename, "rb")) == NULL) {
				printf("File Not Found : %s\n", filename);
				return 0;
			}
			
			// Seek through the BMP header
			fseek(file, 18, SEEK_CUR);

			// Read the width
			sizeX = getint (file);
			printf("Width of : %s : %lu\n", filename, sizeX);		// Testing
			
			// Read the height 
			sizeY = getint (file);
			printf("Height of : %s : %lu\n", filename, sizeY);		// Testing

			// Calculate the size (assuming 24 bits and 3 bytes per pixel
			size = sizeX * sizeY * 3;

			// Read the planes
			planes = getshort(file);
			if (planes != 1) {
				printf("ERROR T02 :Planes is not 1 : %u\n", planes);
				return 0;
			}

			// Read the bpp
			bpp = getshort(file);
			if (bpp != 24) {
				printf("ERROR T03 : Bpp from is not 24 : %u\n", bpp);
				return 0;
			}

			// Seek past the rest of the bitmap header
			fseek(file, 24, SEEK_CUR);

			// Read in the pixel data 
			data = (char *)malloc(size);		// Allocate some space
			if (data == NULL) {
				printf("ERROR T04 : Couldn't allocate the memory for texture data\n");
				return 0;
			}

			if ((i = fread(data, size, 1, file)) != 1) {
				printf("ERROR T05 : Couldn't read texture data from %s\n", filename);
				return 0;
			}

			// Reverse all if the colors (bgr -> rgb)
			for (i = 0; i < size; i+=3) {		// Each pixel
				temp = data[i];					// blue as temp
				data[i] = data[i+2];			// Blue to red
				data[i+2] = temp;				// Red to blue
			}

			return 1;							// Success, yay =D	
		}

	}	// End namspace tex
}		// End namespace nuke

#endif	// __NUKE_TEXTURE__
