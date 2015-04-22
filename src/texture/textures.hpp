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

#ifndef __NUKE_TEXTURES__
#define __NUKE_TEXTURES__

#include <GL/gl.h>				// GLuint
#include <vector>				// Dynamic allocation of textures
#include <initializer_list>		// For list construction
#include "texture.hpp"			// For a single texture

namespace nuke {
	namespace tex {
		/** Class for storing a collection of textures. */
		class Textures
		{
			public:
				vector<GLuint> textures;		/**< Texture container for OpenGL textures. */
			private :
				vector<Texture>	textureInfo;	/**< Container for nuke textures which store the 
													 information to bind to the OpenGL textures. */
				uint numBoundTextures;			/**< The number of textures that have been bound. */
			public :
				/** Default constructor. By design this constructor does not
				 * bind and initialize textures so that there is more control
				 * for when the textures are bound, using the append function
				 * below. This allows textures to be created during
				 * initialization and then bound when appropriate. */
				explicit Textures() throw() : textures(0), textureInfo(0) {}

				/** Append textures to the textures list, and then bind them to
				 * OpenGL using the textureInfo.
				 *
				 * @param newTexNames The names of the new textures to add.
				 */
				void append(initializer_list<const char*> texNames) throw() {
					for (auto& texName : texNames) {				
						textureInfo.emplace_back(texName);			// Add new texture info
						textures.push_back(GLuint(0));				// Add new texture
					}

					// Containers now have the necessary elements, now initalize
					// the GL textures by binding using the textureInfo
					initializeTextures(texNames.size());												
				}
			private :
				/** Function to bind the textures to the to the OpenGL context
				 * using the textureInfo.
				 *
				 * @param numToGenerate The number of textures to generate.
				 */
				void initializeTextures(uint numToGenerate) throw() {
					// Generate numToGenerateTextures starting at the end of the
					// already bound textures
					glGenTextures(numToGenerate, &textures[numBoundTextures]);

					// Now bind the textures that have not been bound 
					for (size_t i = 0; i < numToGenerate; i++) {
						bindTexture(numBoundTextures++);	// Bind texture and increment bound count
					}
				}

				/** Function to bind a specific texture in the texture
				 * container.
				 *
				 * @param index The index of the texture in the texture
				 * container to bind.
				 */
				void bindTexture(uint index) throw() {
					glBindTexture(GL_TEXTURE_2D, textures[index]);		// Bind 2D texture

					// Currently only linear filtering, will add options for
					// more later on (once this is working and tested)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Scale linearly when bigger than tex
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Scale linearly when smaller than texture

					// Parameters :
					//		[2D Texture, Detail Level, Num Components, Tex X size, 
					//		 Tex Y size, Border		 , Color Data	 , Unsigned Byte Data, Texture Data]	
					glTexImage2D(GL_TEXTURE_2D, 0, 3, textureInfo[index].SizeX(), textureInfo[index].SizeY(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureInfo[index].Data());
			}
		};
	}		// End namespace tex
}			// End namespace nuke

#endif		// __NUKE_TEXTURES__


