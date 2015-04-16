/**
 *	Author:		  Rob Clucas
 *
 *	Description : Class defining a particle for the particle system.
 *
 */

#ifndef __NUKE_PARTICLE__
#define __NUKE_PARTICLE__

#include <GL/gl.h>

/**
 *  \struct Definition of a particle.
 */
struct Particle 
{
	vec3 pos;			/**< Particle position. */
	vec3 velocity;		/**< Particle velocity. */
	vec4 color;			/**< Particle color.	*/
	vec3 size;			/**< Particle size.		*/
	float life;			/**< Particle life.		*/
};

#endif // __NUKE_PARTICLE__