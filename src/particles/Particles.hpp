/*
 * Author: Rob Clucas
 *
 * Description: Particles class that is used to group particles.
 *
 */

#ifndef __NUKE_PARTICLES__
#define __NUKE_PARTICLES__

#include "ParticlePolicies.hpp"

/** \class Patricles class used to group numeroud particles and update their
 * states.
 */
template<size_t size, class ParticleType, class InitializerPolicy, class ActionPolicy>
class Particles
{
	public :
		/** Initializer policy used to initalize new particles. */
		InitializerPolicy initializerPolicy;

		/** Action policy used to perform actions on the particles. */
		ActionPolicy actionPolicy;

		/** Construct the particles. */
		explicit Particles() throw() {}

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

		/** Emit a certain number of particles at a certain position. 
		 *
		 * @param _amount The number of particles to emit.
		 * @param _position The location of the emmition.
		 */
		void Emit(const size_t& _amount, const vec3& _position) {
			size_t amount = _amount;									// Can't change _amount
			if ((NumActiveParticles() + amount) > MaxParticles()) {		// Too many particles for system
				amount = MaxParticles() - NumActivePatricles();			// Set to max number of particles that can be added C
			}
			if (amount > 0) {											// Check valid amount
				size_t numActive    = activeCount;						// Counter for the new particles to be added
				size_t activeCount += amount;							// Create more particles
				for (; numActive < activeCount; numActive++) {			// Add particle to the system at end of array
					particleArray[numActive].pos = _position;			
					initializePolicy(particleArray[numActive]);			// Initialize the particle
				}
			}
		}

		/** Update the states of the particles.
		 *  Currently just kill all the dead particles.
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
	private :
		/** The number of currently active particles in the sytem. */
		size_t activeCount;

		/** Array of size particles of ParticleType which make up the system. */
		ParticleType particleArray[size];
};

#endif // __NUKE_PARTICLES__