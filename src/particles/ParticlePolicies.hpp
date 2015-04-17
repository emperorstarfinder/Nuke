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
template <class ParticleType, class LifePolicy, class ColorPolicy,
		  class SizePolicy, class VelocityPolicy, class MovementPolicy>
class CompletePolicy
{
	public :
		LifePolicy lifePolicy;				/**< Life policy for the particle.       */
		ColorPolicy colorPolicy;			/**< Color policy for the particles.	 */
		SizePolicy sizePolicy;				/**< Size policy for the particles.		 */
		VelocityPolicy velocityPolicy;		/**< Velocity policy for the particles.  */
		MovementPolicy movementPolicy;		/**< Movement policy for the particles.  */

		/** Function to prepare all the actions for the policies. */
		inline void PrepareAction() throw() {
			lifePolicy.PrepareAction();
			colorPolicy.PrepareAction();
			sizePolicy.PrepareAction();
			velocityPolicy.PrepareAction();
			movementPolicy.PrepareAction();
		}

		/** Function to initialise the policies. The policies set their
		 * relevant parameters in the particle. 
		 *
		 * @param particle The particle on which the policies act.
		 */
		inline void operator() (ParticleType& particle) const throw() {
			lifePolicy(particle);				// Give particle life
			colorPolicy(particle);				// Give the particle color
			sizePolicy(particle);				// Give the particle size
			velocityPolicy(particle);			// Perform velocity related actions
			movementPolicy(particle);			// Move the particle
		}
};

/*------ ----- ---- --- -- - Null policy - -- --- ---- ----- ------*/

/** 
 * \class Null policy which can be used as a placeholder for when there is no relevant
 * policy. */
template <class ParticleType>
class NullPolicy
{
	public :
		inline void PrepareAction() throw() {}
		inline void operator() (ParticleType& particle) const throw() {}
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
	public :
		/** Constructs the life initializer. */
		explicit LifeInitializer() throw() : minLife(0.1f), maxLife(1.f) {}

		/** Sets the range of the particle's life.
		 *
		 * @param _minLife The min amount of life for the particle.
		 * @param _maxLife The max amount of life for the particle.
		 */
		inline void SetLifeRange(const float& _minLife, const float& _maxLife) throw (double, int) {
			minLife = _minLife;
			maxLife = _maxLife;
		}

		/** Allows random generation of the amount of life for a particle. Where
		 * the random life range is 0.1 -> 1 by default but can be set using the
		 * SetLifeRange function.
		 *
		 * @param particle The particle for which the amount of life should be
		 * randomized.
		 */
		inline void operator() (ParticleType& particle) const throw() {
			particle.life = minLife + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/(maxLife - minLife));
		}	
};

/** 
 * \class Initializer policy class used to initialize the color of a group of
 * particles.
 */
template <class ParticleType>
class ColorInitializer
{
	public : 
		vec4 color;				/**< Color to make the particles. */
	public :
		/** Constructs a default color as white, completely opaque. */
		explicit ColorInitializer() throw() : color(1.f, 1.f, 1.f, 0.f) {}

		/** Set the color that the particles will be made. 
		 *
		 * @param _color The color which the particles will be.
		 */
		inline void SetColor(const vec4& _color) throw() {
			color = _color;
		}

		/** Sets the color of the particle(s).
		 *
		 * @param particle The particle for which the color must be set. 
		 */
		inline void operator() (ParticleType& particle) const throw() {
			particle.color = color;
		}
};


/** 
 * \class Initializer policy class to be used to intialize the size of the
 * particles.
 */
template <class ParticleType>
class SizeInitializer
{
	public:
		vec3 size;				/**< Size of the particle. */
	public:
		/** Construct the size initializer with a default 2D particle with unit
		 * area. */
		explicit SizeInitializer() throw() : size(vec3(1.f, 1.f, 0.f)) {}

		/** Sets the size of a particle which is to be created.
		 *
		 * @param _size The size of the particle (3D)
		 */
		inline void SetSize(const vec3& _size) throw() {
			size = _size;
		}

		/** Sets the particle's size to the size defined in the class.
		 *
		 * @param particle The particle for which the size will be set.
		 */
		inline void operator() (ParticleType& particle) const throw() {
			particle.size = size;
		}
};

/**
 * \class Initializer policy class used to initialize the velocity of a
 * group of particles.
 */
template <class ParticleType>
class VelocityInitializer
{
	public :
		vec3 velocity;			/**< Velocity for the particles. */
	public :
		/** Constructs the velocity initializer with a zero velocity */
		explicit VelocityInitializer() throw() : velocity(vec3(0.f)) {}

		/** Sets the velocity to initialize the particles with.
		 *
		 * @param _velocity The velocity the particles should have.
		 */
		inline void SetVelocity(const vec3& _velocity) throw() {
			velocity = _velocity;
		}

		/** Sets the velocity of the particle on which it acts to be the
		 * velocity defined by the class velocity variable.
		 *
		 * @param particle The particle for which the velocity must be set.
		 */
		inline void operator() (ParticleType& particle) const throw() {
			particle.velocity = velocity;
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
typedef CompletePolicy<Particle, LifeInitializer<Particle>, ColorInitializer<Particle>, SizeInitializer<Particle>,
						VelocityInitializer<Particle>, NullPolicy<Particle> >
						ParticleInitializer;

// Currently only actions for life and for movement
typedef CompletePolicy<Particle, LifeAction<Particle>, NullPolicy<Particle>, NullPolicy<Particle>, 
						NullPolicy<Particle>, MoveAction<Particle> >       
						ParticleAction;

#endif // __NUKE_PARTICLE_POLICIES__
