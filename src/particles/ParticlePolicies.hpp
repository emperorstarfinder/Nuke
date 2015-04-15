/*
 * Author:	Rob Clucas
 *
 * Description: Definition of the policies for Nuke's particle engine.
 *
 */

#include "Particle.h"
#include <cstdlib>				// Rand()

/** 
 * \class Complete policy which defines all the policies which must be used.
 * Simply create a new policy and add it to the template list when more
 * functionality is required.
 */
template <class ParticleType, class LifePolicy>
class CompletePolicy
{
	public :
		LifePolicy lifePolicy;	/**< Life policy for the particle. */

		/** Function to setup the actions of the policies. */

		/** Function to initialise the policies. The policies set their
		 * relevant parameters in the particle. 
		 *
		 * @param particle The particle on which the policies act.
		 */
		inline void operator() (ParticleType& particle) const throw() {
			lifePolicy(particle);				// Give particle life
		}
};


/**
 *  \class Initialize policy class used to initialize the life of a particle.
 */
template <class PaticleType>
class LifeInitializer
{
	public :
		float minLife;			/**< The min amount of life for the particle.	*/
	private :
		float maxLife;			/**< The max amount of life for the particle.	*/
		float lifeDecayRate;	/**< The rate of decay of the life.				*/
	public :
		/** Constructs the life initializer. */
		explicit LifeInitializer() throw() : maxLife(1.f), minLife(0.f), lifeDecayRate(0.001f)  {}

		/** Sets the range of the particle's life.
		 *
		 * @param _minLife The min amount of life for the particle.
		 * @param _maxLife The max amount of life for the particle.
		 */
		inline void SetLifeRange(const float& _mineLife, const float& _maxLife) throw (double, int) {
			minLife = _minLife;
			maxLife = _maxLife;
		}

		/** Sets the rate of life decay for the particle.
		 *
		 * @param _decayRate The speed at which the life should decay
		 */
		inline void SetLifeDecayRate(const float& _decayRate) throw(double, int) {
			lifeDecayRate = _decayRate;
		}

		/** Allows random generation of the amount of life for a particle.
		 *
		 * @param particle The particle for which the amount of life should be
		 * randomized.
		 */
		inline void operator() (ParticleType& particle) const throw() {
			particle.life = minLife + rand() % maxLife;
		}	
};