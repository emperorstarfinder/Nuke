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
				Texture(const string filename) throw();

				/** Descructor, frees memory on exit. */
				~Texture() {
					if (data != NULL) {
						free(data);
						data = NULL;
					}
				}

			private :
				/** Function to load a texture from a bitmap.
				 *
				 * @param filename The name of the file to get the texture from.
				 * @return If the texture was loaded from the bitmap.
				 * */
				bool loadBitmap(const string filename) throw();
		};

		// Constructor when given the texture filename
		Texture::Texture(const string file) throw() {
	
			string filetype = file.substr(file.find(".") + 1);			// Everything after the '.'

			// Use the relevant function for the filetype (using the compile
			// time hash of the string)
			switch (strtoui(filetype.c_str())) {
				
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
		bool Texture::loadBitmap(const string filename) throw() {
			FILE *file;					// File handle
			ulong size;					// Image size (in bytes)
			ulong i;					// Counter
			ushorti planes;				// Number of planes in image (must be 1)
			ushorti bpp;				// Bits per pixel (must be 24)
			char temp;					// Temp col storage for bgr->rgb conversion
	
			// Make sure that the file exists
			if ((file = fopen(filename.c_str(), "rb")) == NULL) {
				cout << "File Not Found : " << filename << endl;
				return 0;
			}
			
			// Seek through the BMP header
			fseek(file, 18, SEEK_CUR);

			// Read the width
			sizeX = getint (file);
			cout << "Width of : " << filename << " : " <<  sizeX << endl;		// Testing
			
			// Read the height 
			sizeY = getint (file);
			cout << "Height of : " << filename << " : " <<  sizeY << endl;		// Testing

			// Calculate the size (assuming 24 bits and 3 bytes per pixel
			size = sizeX * sizeY * 3;

			// Read the planes
			planes = getshort(file);
			if (planes != 1) {
				cout << "ERROR T02 :Planes is not 1 : " << planes << endl;
				return 0;
			}

			// Read the bpp
			bpp = getshort(file);
			if (bpp != 24) {
				cout << "ERROR T03 : Bpp from is not 24 : " << bpp << endl;
				return 0;
			}

			// Seek past the rest of the bitmap header
			fseek(file, 24, SEEK_CUR);

			// Read in the pixel data 
			data = (char *)malloc(size);		// Allocate some space
			if (data == NULL) {
				cout << "ERROR T04 : Couldn't allocate the memory for texture data" << endl;
				return 0;
			}

			if ((i = fread(data, size, 1, file)) != 1) {
				cout << "ERROR T05 : Couldn't read texture data from " << filename << endl;
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
