/*
 *  Life Policy
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

#ifndef __GFX_PARTICLE_LIFE_POLICY
#define __GFX_PARTICLE_LIFE_POLICY

using namespace std;

namespace gfx {
	namespace part {
		/*
		 * ===============================================================
		 * Class		: LifeInitializer
		 *
		 * Description	: Gives a particle life upon creation.
		 * ===============================================================
		 */
		template <class ParticleType>
		class gfxLifeInitializer
		{
			public:
				float minLife;			// Minimum amount of life for a particle.
			public:
				/*
				 * =======================================================
				 * Function		: gfxLifeInitializer
				 *
				 * Description	:  Constructs the life initializer
				 * =======================================================
				 */
				explicit gfxLifeInitializer() throw() : 
					minLife( 0.1f ), maxLife( 1.0f ) {}

				/*
				 * =======================================================
				 * Function		: SetLifeRange
				 *
				 * Description	: Sets the range of life values that a 
				 *				  particle can have.
				 *
				 * Params		: _minLife	: Minimum amount of life
				 *				: _maxLife	: maximum amount of life
				 * =======================================================
				 */
				inline void SetLifeRange( const float& _minLife, const float& _maxLife ) throw () {
					minLife = _minLife;
					maxLife = _maxLife;
				}

				/*
				 * =========================================================
				 * Function		: Operator ()
				 *
				 * Description	: Applies the class life variables to the 
				 *				  particle, settings its life. 
				 *
				 * Params		: particle	: The particle for which to
				 *							  initialize life.
				 * =========================================================			
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.life = minLife + 
						( static_cast<float>( rand() ) / 
						  static_cast<float>( RAND_MAX/(maxLife - minLife) ));
				}	
			private:
				float maxLife;			// Maximum amount of life for a particle
	
		};

		/*
		 * ====================================================================
		 * Class		: gfxLifeAction 
		 *
		 * Description	: Defines actions that act on particle life.
		 * ====================================================================
		 */
	    template <class ParticleType>
		class gfxLifeAction
		{
			public:
				float decayRate;			// Rate at which life decays
			public:
				/*
				 * ===========================================================
				 * Function		: gfxLifeAction
				 *
				 * Description	: Constructs a life action policy, with life 
				 *				  decaying very slowly. 
				 * ===========================================================
				 */
				explicit LifeAction() throw() : decayRate( 0.001f ) {}

				/*
				 * ===========================================================
				 * Function		: PrepareAction
				 *
				 * Description	: Performs any necessary preparation for a 
				 *				  the life actions. 
				 * ===========================================================
				 */
				inline void PrepareAction() throw() {}

				/*
				 * ===========================================================
				 * Function		: Operator ()
				 *
				 * Description	: Applies the life actions to particle
				 *
				 * Params		: particle	: The particle on which the life 
				 *							  actions act on.
				 * ===========================================================
				 */
				inline void operator() ( ParticleType& particle ) const throw() {
					particle.life -= decayRate;
				}
		};
	}			// End namespace part
}				// End namespace gfx

#endif			// __GFX_PARTICLE_LIFE_POLICY__
	
	   	   
	
		