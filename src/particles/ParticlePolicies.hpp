/*
 *  Particle Policies for Nuke.
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

#ifndef __NUKE_PARTICLE_POLICIES__
#define __NUKE_PARTICLE_POLICIES__

#include "Particle.h"
#include <cstdlib>				// rand()
#include <math.h>				// fmod()

// Note: The typedefs which define the instances of these policy classes are 
//		 at the bottom of the file so that no forward declaration is needed 
//		 which will result in clutter at the top of the file.

/** 
 * \class Complete policy which defines all the policies which must be used.
 * Simply create a new policy and add it to the template list when more
 * functionality is required.
 */
template <class ParticleType, class LifePolicy, class MovementPolicy>
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
};

/*------ ----- ---- --- -- - Initializer Policies - -- --- ---- ----- ------*/

/**
 *  \class Initialize policy class used to initialize the life of a particle.
 */
template <class ParticleType>
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
			particle.life = fmod(minLife + rand(), maxLife);
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

// Typedefs so that the rest of the program looks nice
typedef CompletePolicy<Particle, LifeInitializer<Particle>, NullPolicy<Particle> >  particleInitializer;
typedef CompletePolicy<Particle, LifeAction<Particle>, MoveAction<Particle> >       particleAction;

#endif // __NUKE_PARTICLE_POLICIES__
