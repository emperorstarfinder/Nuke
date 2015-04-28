/*
 *  Time class that stores global run time that can to used when t
 *  is needed for equations.
 *
 *  Copyright (C) 2015 Rob Clucas robclu1818@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT AN_size.y WARRANTY; without even the implied warranty of
 *  MERCHANTABILIT_size.y or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  _size.you should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation,
 *	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __GFX_TIME__
#define __GFX_TIME__

#include <ctime>
#include <chrono>

using namespace std::chrono;

typedef high_resolution_clock				Clock;
typedef high_resolution_clock::time_point	Time;
typedef unsigned long long					ullong;

namespace gfx {
	namespace time {
		/*
		 * =======================================================================
		 * Class		: gfxTime
		 *
		 * Description	: Keeps the current time as the time since the program
		 *                started, and the iteration time as the time since the 
		 *                last call of the main loop.
		 * =======================================================================
		 */
		class gfxTime 
		{
			private:
				static Time startTime;	// Time at the start of the program
				static Time	prevTime;	// Time of last iteration
			public:
				/*
				 * ==============================================================
				 * Function			: gfxTime
				 *
				 * Description		: Constructs the gfxTime class.
				 * ==============================================================
				 */
				explicit gfxTime() throw() {}

				/* 
				 * ==============================================================
				 * Function			: ResetTime
				 *
				 * Description		: Resets all the time variables to the
				 *					  current time.
				 * ==============================================================
				 */
				inline void ResetTime() throw() {
					startTime = Clock::now();
					prevTime  = Clock::now();
				}

				/* 
				 * ==============================================================
				 * Function			: TotalTime
				 *
				 * Description		: Returns the total time that has passed 
				 *					  since the start of the program in
				 *					  nanoseconds
				 * ==============================================================
				 */
				inline ullong TotalTime() const throw() {
					return duration_cast<nanoseconds>( Clock::now() - startTime ).count();
				}

				/*
				 * ==============================================================
				 * Function			: TotalTimef
				 *
				 * Description		: Returns the total time that has passed
				 *					  since the time was reset, in seconds.
				 * ==============================================================
				 */
				inline float TotalTimef() const throw() {
					return ( static_cast<float>( 
								duration_cast<nanoseconds>( 
									Clock::now() - startTime ).count() ) / 10000000000.f );
				}

				/*
				 * ===============================================================
				 * Function			: DeltaTime 
				 *
				 * Description		: Returns the number of nanoseconds since
				 *					  the last call to ( DeltaTime | 
				 *					  DeltaTimef ).
				 * ================================================================
				 */
				inline ullong DeltaTime() throw() {
					ullong iterTime = duration_cast<nanoseconds>( Clock::now() - prevTime ).count();
					prevTime = Clock::now();		// Next iter prev time is this time
					return iterTime;
				}

				/* 
				 * ================================================================
				 * Function			: DeltaTimef
				 *
				 * Description		: Returns the time since the last call to 
				 *					  ( DeltaTimef | DeltaTime ) in secs
				 * ================================================================
				 */
				inline float DeltaTimef() throw() {
					float iterTime = static_cast<float>( duration_cast<nanoseconds> (
								Clock::now() - prevTime ).count() ) / 1000000000.f;
					prevTime = Clock::now();
					return iterTime;
				}
		};
	}			// End namespace time
}				// End namespace gfx

#endif			// __GFX_TIME__

