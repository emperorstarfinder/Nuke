/*
 *  Particles class for Nuke.
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

#ifndef __NUKE_PARTICLES__
#define __NUKE_PARTICLES__

#include "ParticlePolicies.hpp"
#include "../texture/textures.hpp"
#include "../rederer/buildlists.hpp"		// For creating build lists
#include "../shape/shape.hpp"				// For Shape class

using namespace nuke::tex;		    // For Textures class
using namespace nuke::shape;		// For Shape and ShapeHash
using namespace nuke::rend;			// For BuildList

namespace nuke {
	namespace part {
		/** 
		 * \class Patricles class used to group numeroud particles and update their
		 * states.
		 */
		template<size_t size, class ParticleType, class InitializerPolicy, class ActionPolicy>
		class Particles
		{
			public:
				/** Initializer policy used to initalize new particles. */
				InitializerPolicy initializerPolicy;

				/** Action policy used to perform actions on the particles. */
				ActionPolicy actionPolicy;

				/** Construct the particles. */
				explicit Particles() throw() : activeCount(0) {}

				/** Destruct the particles. */
				~Particles() throw() {}

				/** Retuns the maximum number of particles for the system. */
				inline const size_t MaxParticles() const throw() {
					return size;
				}

				/** Returns the number of currently active (alive) particles. */
				inline const size_t NumActiveParticles() const throw() {
					return activeCount;
				}

				/** Returns a pointer to the particle array representing the system. */
				inline const ParticleType* GetParticles() const throw() {
					if (NumActiveParticles() == 0) {
						return 0;
					}
					return particleArray;
				}

				/** Binds textures to OpenGL context which are supplied as
				 * arguments.
				 *
				 * @param texNames The names of the texture to bind.
				 */
				inline void BindTextures(initializer_list<const char*> texNames) throw() {
					textures.append(texNames);
				}

				/** Emit a certain number of particles at a certain position. 
				 *
				 * @param _amount The number of particles to emit.
				 * @param _position The location of the emmition.
				 */
				void Emit(const size_t& _amount, const vec3& _position) {
					size_t amount = _amount;									// Can't change _amount
					if ((NumActiveParticles() + amount) > MaxParticles()) {		// Too many particles for system
						amount = MaxParticles() - NumActiveParticles();			// Set to max number of particles that can be added C
					}
					if (amount > 0) {											// Check valid amount
						size_t numActive = activeCount;							// Counter for the new particles to be added
						activeCount += amount;									// Create more particles
						for (; numActive < activeCount; numActive++) {			// Add particle to the system at end of array
							particleArray[numActive].pos = _position;			
							initializerPolicy(particleArray[numActive]);		// Initialize the particle
						}
					}
				}

				/** 
				 * Update the states of the particles.
				 * This involves performing all the actions defined by the action
				 * policies and getting rid of 'dead' pparticles.
				 */
				void Update() throw() {
					actionPolicy.PrepareAction();								// Prepare all action policies
					for (size_t count = 0; count < activeCount; ) {				// Kill all dead particles
						actionPolicy(particleArray[count]);						// Apply all the action policies to the particle
						if (particleArray[count].life < 0) {			
							// Particle is dead so swap it to the last active article in the array
							particleArray[count] = particleArray[activeCount];
							activeCount--;										// Decrease number of active particles
						}
						else {
							// If it isn't dead, move to the next one (swapped one could
							// also be dead and we can't miss any dead particles)
							++count;
						}
					}
				}

				/** Compiles buildlists for the shapes that the partices need. 
				 * This should be done before the main loop execution to take
				 * advantage of the rendering speed it will result in.
				 */
				void BuildShapes() throw() {
					// For each active particle
					for (size_t part = 0; part < activeCount; part++) {
						// Check to see if a build list has been compiled for this shape
						if (shapes.find(particleArray[part].shape) == shapes.end()) {
								// If not found hen we need to add this shape 
								// (GLuint) with the key as the particle shape
								shapes.emplace(particleArray[part].shape, static_cast<GLuint>(shapes.size()));

								// Now build a list for that shape
								BuildList(particleArray[part].shape.size, &shapes[particleArray[part].shape]);
								}
					}
				}

				/** Draws the particles in the particle group to the screen. */
				void Draw() throw() {
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);							// Clear relevant buffers
	
					for (size_t part = 0; part < activeCount; part++) {							// For each active particle
						glBindTexture(GL_TEXTURE_2D, textures[particleArray[part].texture]);	// Bind the relevant texture

						// Move the particle (No rotation at the moment)
						glLoadIdentity();
						glTranslatef(particleArray[part].pos.x, particleArray[part].ppos.y, particleArray[part].pos.z);
						glColor4fv(particleArray[part].color);					// Set the color of the particle
						glCallList(shapes[particleArray[part].shape]);			// Get and draw the shape
					}
					
			private:
				/** The number of currently active particles in the sytem. */
				size_t activeCount;

				/** Array of size particles of ParticleType which make up the system. */
				ParticleType particleArray[size];

				/** Textures that are used by the group of particles. */
				Textures textures;

				/** Hashtable of shapes that need to be drawn. This is so that
				 * we only need to compile a buildlist each time a particle has
				 * a different shape.
				 */
				unordered_map<Shape, Gluint, ShapeHash> shapes;
		};
	}
}

#endif // __NUKE_PARTICLES__