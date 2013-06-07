/*
 * Smart profiler
 *
 * Copyright (c) 2011-2013 FORZEFIELD Studios S.L.
 * Copyright (c) 2013 Manuel Martínez-Almeida
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef __SMART_PROFILER__
#define __SMART_PROFILER__

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>

typedef struct _profile_buf {
	const char *title;
	unsigned long long total;
	unsigned long long index;
	struct timeval timestamp;
} profile_buf;


inline profile_buf __initProfile(const char *title, unsigned long long total)
{
	profile_buf state;
	state.title = title;
	state.total = total;
	state.index = 0;
	gettimeofday(&state.timestamp, NULL);
	return state;
}


inline bool __profile(profile_buf *state)
{
	if(state->index != state->total)
		return true;
		
	else
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		
		unsigned long long totalTime = (now.tv_sec - state->timestamp.tv_sec)*1000000 + (now.tv_usec - state->timestamp.tv_usec);
		double iterationTime = (double)totalTime/(double)state->total;
		
		printf(
		"%s:\n"
		"	- Iterations: %llu\n"
		"	- Total time: %llu μs  (%f s)\n"
		"	- Iteration time: %f μs  (%f s)\n\n",
		state->title,
		state->total,
		totalTime, (totalTime/1000000.0),
		iterationTime, (iterationTime/1000000.0));
		
		return false;
	}
}

#define PROFILE_N(__TEXT__, __TOTAL__) \
for(profile_buf buff = __initProfile(__TEXT__, __TOTAL__); __profile(&buff); ++buff.index)

#define PROFILE(__TEXT__) PROFILE_N(__TEXT__, 100000000)


#endif