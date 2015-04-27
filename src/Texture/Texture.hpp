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

#ifndef __GFX_TEXTURE__
#define __GFX_TEXTURE__

#include <string.h>
#include <stdio.h>
#include "TextureUtils.hpp"

typedef unsigned long ulong;
typedef unsigned short int ushorti;

using namespace std;

/*
 * =========================================================================
 * Function		: Strtoui
 *
 * Description	:  Compute the hash of a string at compile time, returns 
 *				   an integer representing the string.
 *
 * Params		: str	: String to hash.
 *				: hash	: Element of str to use.
 * =========================================================================
 */ 
constexpr unsigned int Strtoui(const char* str, int hash = 0) 
{
	return !str[hash] ? 5381 : (Strtoui(str, hash + 1) * 33) ^ str[hash];
}

namespace gfx {
	namespace tex {
		/*
		 * =================================================================
		 * Class		: gfxTexture
		 *
		 * Description	: Stores images data and size to use as an OpenGL
		 *				  texture.
		 * =================================================================
		*/
		class gfxTexture 
		{
			public:
				/*
				 * ==========================================================
				 * Function		: gfxTexture
				 *
				 * Description	: Default constructor, initializes Texture 
				 *				  variables.
				 * ==========================================================
				 */
				explicit gfxTexture() throw() : sizeX(0), sizeY(0), data(NULL) {}

				/* 
				 * ==========================================================
				 * Function		: gfxTexture
				 *
				 * Description	: Loads the image at the location provided by
				 *				  the filename as a Texture.
				 *
				 * Params		: filename : The name of the file to get the 
				 *							 Texture information from.
				 *
				 * Additional	: The filename should be specified in the
				 *				  following	format : <Dir><Name><Size>.<Type>
				 *				  where :
				 *					
				 *						Name = Texture name
				 *						Size = 3 digits giving texture width
				 *						Type = Image file type
				 * ==========================================================
				 */
				explicit gfxTexture(const char* filename) throw();

				/* 
				 * ==========================================================
				 * Function		: ~gfxTexture 
				 *
				 * Description	: Frees Texture data when Texture goes out of
				 *				  scope. 
				 * ==========================================================
				 */
				~gfxTexture() throw() {
					if (data != NULL) {
						free(data);
						data = NULL;
					}
				}

				/*
				 * ===========================================================
				 * Function		: SizeX
				 *
				 * Description	: Returns the size (number of pixels) in the x
				 *				  direction of the Texture (width).
				 * ===========================================================
				 */
				inline ulong SizeX() const { return sizeX; }
	
				/*
				 * ===========================================================
				 * Function		: SizeY
				 *
				 * Description	: Returns the size (number of pixels) in the y
				 *				  direction of the Texture (height).
				 * ===========================================================
				 */
				inline ulong SizeY() const { return sizeY; }

				/*
				 * ===========================================================
				 * Function		: Data
				 *
				 * Description	: Returns a pointer to the Texture data.
				 * ===========================================================
				 */
				inline char* Data() const { return data; }

			private:
				ulong sizeX;		// Texture x dimension size in pixels (width) 
				ulong sizeY;		// Texture y dimension size in pixels (height) 
				char* data;			// Pointer to the Texture data

			private:
				/* 
				 * ===========================================================
				 * Function		: LoadBitmap
				 *
				 * Description	: Loads a bitmap image into a Texture object,
				 *				  returns if the Texture was loaded.
				 *
				 * Params		: filename : The name of bitmap image file to 
				 *							 load as a Texture.
				 * ============================================================
				 * */
				bool LoadBitmap(const char* filename) throw();
		};

		/*
		 * ========================================================================== 
		 *								Implementation  
		 * ==========================================================================
		 */

		gfxTexture::gfxTexture(const char* file) throw() {
			const char* filetype = strchr(file, '.');	// Get .<filetype>
			switch (Strtoui(++filetype)) {				// "++" will give <filetype>
				case Strtoui("bmp"):					// Filetype == bitmap
					if (!LoadBitmap(file)) {
						printf("ERROR T01 : Texture could not be loaded from bitmap file %s\n", file);
					}
					break;
				default:
					break;
			}
		}

		bool gfxTexture::LoadBitmap(const char* filename) throw() {
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
			sizeX = GetInt(file);
			printf("Width of : %s : %lu\n", filename, sizeX);		// Testing
			
			// Read the height 
			sizeY = GetInt(file);
			printf("Height of : %s : %lu\n", filename, sizeY);		// Testing

			// Calculate the size (assuming 24 bits and 3 bytes per pixel)
			size = sizeX * sizeY * 3;

			// Read the planes
			planes = GetShort(file);
			if (planes != 1) {
				printf("ERROR T02 :Planes is not 1 : %u\n", planes);
				return 0;
			}

			// Read the bpp
			bpp = GetShort(file);
			if (bpp != 24) {
				printf("ERROR T03 : Bpp from is not 24 : %u\n", bpp);
				return 0;
			}

			// Seek past the rest of the bitmap header
			fseek(file, 24, SEEK_CUR);

			// Read in the pixel data 
			data = (char *)malloc(size);	
			if (data == NULL) {
				printf("ERROR T04 : Couldn't allocate the memory for texture data\n");
				return 0;
			}

			if ((i = fread(data, size, 1, file)) != 1) {
				printf("ERROR T05 : Couldn't read texture data from %s\n", filename);
				return 0;
			}

			// Reverse colors (bgr -> rgb)
			for (i = 0; i < size; i+=3) {		// Each pixel
				temp		= data[i];			// blue as temp
				data[i]		= data[i+2];		// Blue to red
				data[i+2]	= temp;				// Red to blue
			}

			return 1;						
		}

	}	// End namspace tex
}		// End namespace gfx

#endif	// __GFX_TEXTURE__
