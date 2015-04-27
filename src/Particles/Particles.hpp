/*
 *  Particles class.
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

#ifndef __GFX_PARTICLES__
#define __GFX_PARTICLES__

#include "Particle.h"
#include "../Texture/Textures.hpp"
#include "../Render/BuildList.hpp"		// For creating build lists
#include "../Shape/Shape.hpp"			// For Shape class

using namespace gfx::tex;				// For Textures class
using namespace gfx::shape;				// For Shape and ShapeHash
using namespace gfx::rend;				// For BuildList

namespace gfx {
	namespace part {
		/*
		 * ==================================================================
		 * Class		: Particles 
		 *
		 * Description	: Class that is a container for holding any number of 
		 *				  gfxParticle objects. 
		 * ===================================================================
		 */
		template<size_t size            , class ParticleType, 
				 class InitializerPolicy, class ActionPolicy>
		class gfxParticles
		{
			public:
				InitializerPolicy initializerPolicy;	// Defines initial state
				ActionPolicy	  actionPolicy;			// Defines actions 
			public:
				/*
				 * ===========================================================
				 * Function		: Particles 
				 *
				 * Description	: Default constructor for gfxParticles, sets 
				 *				  no particles to be active. 
				 * ===========================================================
				 */
				explicit gfxParticles() throw() : activeCount(0) {}

				/*
				 * ===========================================================
				 * Function		: ~gfxParticles
				 *
				 * Description	: Destructs particles
				 * ===========================================================
				 */
				~gfxParticles() throw() {}

				/*
				 * ===========================================================
				 * Function		: MaxParticles
				 *
				 * Description	: Returns the maximum number of particles for 
				 *				  allowed for the gfxParticle system. 
				 * ===========================================================
				 */
				inline const size_t MaxParticles() const throw() {
					return size;
				}

				/*
				 * ============================================================
				 * Function		: NumActiveParticles
				 *
				 * Description	:  Returns the number of currently active 
				 *				   particles, particles with life.
				 * ============================================================
				 */
				inline const size_t NumActiveParticles() const throw() {
					return activeCount;
				}

				/*
				 * ============================================================
				 * Function		: GetParticles 
				 *
				 * Description	: Returns a pointer to the particle array, 
				 *				  which holds systems gfxParticle elements.
				 * ============================================================
				 */
				inline const ParticleType* GetParticles() const throw() {
					if ( NumActiveParticles() == 0 ) {
						return 0;
					}
					return particleArray;
				}

				/*
				 * ============================================================
				 * Function		: AddTextures
				 *
				 * Description	: Adds gfxTexture objects to the container of 
				 *				  gfxTextures.
				 *
				 * Params		: texNames	: A list of texture names (names 
				 *							  of the inamge files) to add as
				 *							  gfxTextures.
				 * ============================================================
				 */
				inline void AddTextures( initializer_list<const char*> texNames ) throw() {
					textureArray.Append( texNames );
				}

				/*
				 * ===========================================================
				 * Function		: Emit
				 *
				 * Description	: Emits (makes active) a certain number of 
				 *				  gfxParticle objects at a certain position.
				 *
				 * Params		: _amount	: The number of gfxPartice 
				 *							  objectst to make active.
				 *				: _position	: The position of the gfxParticle
				 *							  objects. 
				 * ===========================================================
				 */
				void Emit( const size_t& _amount, const vec3& _position ) {
					size_t amount = _amount;									
					if ( (NumActiveParticles() + amount) > MaxParticles() ) {	// Too many particles for system
						amount = MaxParticles() - NumActiveParticles();			// Set to max number of particles that can be added 
					}
					if ( amount > 0 ) {										
						size_t numActive = activeCount;							// Counter for the new particles to add
						activeCount += amount;									// Allow for more particles
						for (; numActive < activeCount; numActive++) {			
							particleArray[numActive].pos = _position;			
							initializerPolicy( particleArray[numActive] );		// Initialize the particle
						}
					}
				}

				/*
				 * ===========================================================
				 * Function		: Update 
				 *
				 * Description	: Updates the states of all active gfxParticle 
				 *				  objects. Each action is defined by a policy.
				 * ===========================================================
				 */
				void Update() throw() {
					actionPolicy.PrepareAction();								
					for ( size_t count = 0; count < activeCount; ) {			
						actionPolicy( particleArray[count] );					// Apply all action policies
						if ( particleArray[count].life < 0 ) {			
							particleArray[count] = particleArray[activeCount];	// Swap dead particle with last known active particle
							activeCount--;										// Decrease number of active particles
						} else {
							// If it isn't dead, move to the next one (swapped one could
							// also be dead and we can't miss any dead particles)
							++count;
						}
					}
				}

				/*
				 * ============================================================
				 * Function		: BuildShapes
				 *
				 * Description	: Compiles buildlists for the shapes that the 
				 *				  the gfxParticle objectsneed. 
				 * ============================================================
				 */
				void BuildShapes() throw() {
					for ( size_t part = 0; part < activeCount; part++ ) {
						if ( shapes.find( particleArray[part].shape ) == shapes.end() ) {	// Check if there is a build list for this shape
								// If not found hen we need to add this shape to
								// the shapes container with the gfxShape as  the key
								shapes.emplace( particleArray[part].shape, static_cast<GLuint>( shapes.size() ) );

								// gfxParticle shape is definitely in shapes container
								BuildList( particleArray[part].shape.type,					// Type of shape to build
										   particleArray[part].shape.size,					// Size of shape to build
										   &shapes[particleArray[part].shape] );			// Pointer to built shape handle
						}
					}
				}

				/*
				 * =============================================================
				 * Function		: Draw
				 *
				 * Description	:  Draws all active gfxParticle objects onto 
				 *				   the screen.
				 * =============================================================
				*/
				void Draw() throw() {
					if ( textureArray.textures.size() > 0 ) {						// Make sure that there is a texture to bind		
						for ( size_t part = 0; part < activeCount; part++ ) {							
							glBindTexture( GL_TEXTURE_2D, textureArray.textures[particleArray[part].texture] );	
							glLoadIdentity();										// Back to origin
							glTranslatef (particleArray[part].pos.x,				// Move to particle position
										  particleArray[part].pos.y, 
										  particleArray[part].pos.z);
							glColor4f( particleArray[part].color.r,					// Set the color
									   particleArray[part].color.g,					
									   particleArray[part].color.b,					
									   particleArray[part].color.a);				
							glCallList( shapes[particleArray[part].shape] );		// Call the compiles shape
						}
					}	else {
						// Should do some error checking here 
						printf("Error P01 : Not texture loaded for drawing.");
					}
				}
			private:
				size_t						activeCount;			// Number of active particles
				ParticleType				particleArray[size];	// Array holding the particles
				gfxTextures					textureArray;			// Textures used by the particles
				unordered_map<gfxShape, 
							  GLuint, 
							  gfxShapeHash> shapes;					// Shapes used by the particles
		};
	}
}

#endif // __GFX_PARTICLES__