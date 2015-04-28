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
		typedef high_resolution_clock				Clock;
		typedef high_resolution_clock::time_point	Time;

		static Time startTime = Clock::now();	// Time at the start of the program
		static Time	prevTime  = Clock::now();	// Time of last iteration

		/* 
		 * ==============================================================
		 * Function			: ResetStartTime
		 *
		 * Description		: Resets the program start time.
		 * ==============================================================
		 */
		inline void ResetStartTime() throw() {
			startTime = Clock::now();
		}

		/*
		 * =============================================================
		 * Function			: ResetDeltaTime
		 *
		 * Description		: Resets the time which is used to calculate 
		 *					  the next timestep.
		 * =============================================================
		 */
		inline void ResetDeltaTime() throw() {
			prevTime = Clock::now();
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
		inline ullong TotalTime() throw() {
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
		inline float TotalTimef() throw() {
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
	}			// End namespace time
}				// End namespace gfx

#endif			// __GFX_TIME__

