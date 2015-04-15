/*
 * Author: Rob Clucas
 *
 * Description: Particles class that is used to group particles.
 *
 */

#include "ParticlePolicies.hpp"

/** \class Patricles class used to group numeroud particles and update their
 * states.
 */
template<size_t size, class ParticleType>
class Particles
{
	public :
		/** Construct the particles. */
		explicit Particles() throw() {}

		/** Destruct the particles. */
		~Particles() throw() {}

		/** Emit a certain number of particles at a certain position. 
		 *
		 * @param _amount The number of particles to emit.
		 * @param _position The location of the emmition.
		 */
		void Emit(const size_t _amount, const vec3& _position) {
		}

		/** Update the states of the particles. */
		void Update() throw() {
		}
	private :
};