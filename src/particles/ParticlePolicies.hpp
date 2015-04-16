/*
 * Author:	Rob Clucas
 *
 * Description: Definition of the policies for Nuke's particle engine.
 *
 */

#ifndef __NUKE_PARTICLE_POLICIES__
#define __NUKE_PARTICLE_POLICIES__

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

/*------ ----- ---- --- -- - Null policy - -- --- ---- ----- ------*/

/** 
 * \class Null policy which can be used as a placeholder for when there is no relevant
 * policy. */
template <class ParticleType>
class NullPolicy
{
}

/*------ ----- ---- --- -- - Initializer Policies - -- --- ---- ----- ------*/

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
		explicit LifeInitializer() throw() : maxLife(1.f), minLife(0.f)  {}

		/** Sets the range of the particle's life.
		 *
		 * @param _minLife The min amount of life for the particle.
		 * @param _maxLife The max amount of life for the particle.
		 */
		inline void SetLifeRange(const float& _minLife, const float& _maxLife) throw (double, int) {
			minLife = _minLife;
			maxLife = _maxLife;
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

/*------ ----- ---- --- -- - Action Policies - -- --- ---- ----- ------*/

/** 
 * \class Action policy used to perform the actions applied to particle life.
 */
template <class ParticleType>
class LifeAction
{
	public:
		/** The rate at which the life of a particle decays. */
		float decayRate;
	public:
		/** Constructs a life action policy. */
		explicit LifeAction() throw() : decayRate(0.001f) {}

		/** Performs any necessary preparation for a life policy. */
		inline void PrepareAction() throw() {}

		/** Defines the actions which must be performed when the life action
		 * acts on the particle.
		 *
		 * @param particle The particle on which the life action acts. */
		inline void operator() (ParticleType& particle) const throw() {
			particle.life -= decayRate;
		}
};
/** 
 * \class Action policy class used to perform particle movement.
 */
template <class ParticleType>
class MoveAction
{
	public:
		/** Performs any necessary setup before the action can act. */
		inline void PrepareAction() throw() {}

		/** Defines the actions which must be performed when the life action
		 * acts on the particle. 
		 *
		 * @param particle The particle on which the life action must act. */
		inline void operator() (ParticleType& particle) const throw() {
			// Opengl vectors act elementwise, update all dimensions in one call
			particle.pos += particle.velocity;
		}
};

#endif // __NUKE_PARTICLE_POLICIES__
