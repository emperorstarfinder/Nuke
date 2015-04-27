/*
 *  Stores and loads a collection of Textures.
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

#ifndef __GFX_TEXTURES__
#define __GFX_TEXTURES__

#include <GL/gl.h>				// GLuint
#include <vector>				// Dynamic allocation of textures
#include <initializer_list>		// For list construction
#include "Texture.hpp"			// For a single texture

namespace gfx {
	namespace tex {
		/*
		 * ===================================================================================
		 * Class		: gfxTextures 
		 *
		 * Description	: Stores a collection of gfxTexture objects.
		 * ===================================================================================
		 */
		class gfxTextures
		{
			public:
				vector<GLuint>		textures;			// Textures used for drawing
			public:
				/*
				 * ============================================================================
				 * Function			: Default gfxTexture
				 *
				 * Description		: Allocates the textures and textureInfo
				 *					  variables to be be NULL.
				 * ============================================================================
				 */
				explicit gfxTextures() throw() : textures(0), textureInfo(0) {}

				/*
				 * =============================================================================
				 * Function			:  Append 
				 *
				 * Description		: Appends textures to the textures variable, and textureInfo 
				 *					  variables by creating gfxTexture objects using the filenames 
				 *					  provided.
				 * 
				 * Params			: texNames	: Names of the texture files to load texture data 
				 *								  from as gfxTexture objects.
				 * ==============================================================================
				 */
				void Append(initializer_list<const char*> texNames) throw() {
					GLuint handle = 0;							// Handle for the added texture
					for (auto& texName : texNames) {				
						textureInfo.emplace_back(texName);		// Add new texture info
						textures.push_back(handle++);			// Add new texture
					}
					BindTextures(texNames.size());				// Initialize the textures								
				}
			private:
				vector<gfxTexture>	textureInfo;			// Information for the textures
				uint				numBoundTextures;		// Number of textures bound to GL
			private:
				/*
				 * ==============================================================================
				 * Function			: InitializeTextures
				 *
				 * Description		: Binds gfxTexture objects texture data to OpenGL context.
				 *
				 * Params			: numToBind		: The number of textures to bind to the OpenGL 
				 *									  context.
				 * ==============================================================================
				 */
				void BindTextures(uint numToBind) throw() {
					// Generate numToBind OpenGL textures at end of already bound textures
					glGenTextures(numToBind, &textures[numBoundTextures]);
					for (size_t i = 0; i < numToBind; i++) {
						BindTexture(numBoundTextures++);			// Bind unbound textures
					}
				}

				/*
				 * ===============================================================================
				 * Function			:  BindTexture
				 *
				 * Description		: Binds a specific gfxTexture to the OpenGL context.
				 * 
				 * Params			: index		: The index of the texture in the textures 
				 *								  gfxTextures class variable to bind.
				 * ===============================================================================
				 */
				void BindTexture(uint index) throw() {
					glBindTexture(GL_TEXTURE_2D, textures[index]);						// Bind 2D texture
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Scale linearly when bigger than tex
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Scale linearly when smaller than texture

					// Parameters :
					//		[2D Texture, Detail Level, Num Components, Tex X size, 
					//		 Tex Y size, Border		 , Color Data	 , Unsigned Byte Data, Texture Data]	
					glTexImage2D(GL_TEXTURE_2D, 0	  , 3				, textureInfo[index].SizeX(), textureInfo[index].SizeY(),
						         0			  , GL_RGB, GL_UNSIGNED_BYTE, textureInfo[index].Data());
			}
		};
	}		// End namespace tex
}			// End namespace gfx

#endif		// __GFX_TEXTURES__


