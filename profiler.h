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

#if (defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__))
#define __PROFILE_IN_WINDOWS
#endif


#include <stdio.h>
#include <stdlib.h>

#ifdef __PROFILE_IN_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#endif



struct __profile_buf {
	long long index;
	long long total;
	union {
		double time;
#ifdef __PROFILE_IN_WINDOWS
		LARGE_INTEGER timestamp;
#else
		struct timeval timestamp;
#endif
	};
	const char *title;
};


static struct __profile_buf* __profileHistory = NULL;
static int __profileCapacity = 0;
static int __profileCurrent = 0;


static struct __profile_buf __profileInit(const char *title, long long total)
{
	if(__profileCurrent >= __profileCapacity) {
		if(__profileCapacity == 0)
			__profileCapacity = __profileCurrent+10;
		else
			__profileCapacity = __profileCurrent*2;
		
		// realloc calls malloc() internally if the original buffer is NULL.
		__profileHistory = (struct __profile_buf*)realloc(__profileHistory, sizeof(struct __profile_buf)*__profileCapacity);
	}
	
	struct __profile_buf state;
	state.title = title;
	state.total = total;
	state.index = 0;
#ifdef __PROFILE_IN_WINDOWS
	QueryPerformanceCounter(&state.timestamp);
#else
	gettimeofday(&state.timestamp, NULL);
#endif
	return state;
}


static void __profilePrint(const struct __profile_buf *state)
{
	double iterationTime = state->time/state->total;
	
	printf(
		   "%s:\n"
		   "	- Iterations: %lld\n"
		   "	- Total time: %.3f ms (%.6fs)\n"
		   "	- Iteration time: %.8f ms (%.6fs)\n\n",
		   state->title,
		   state->total,
		   state->time, (state->time/1000.0),
		   iterationTime, (iterationTime/1000.0));
}


static inline char __profile(struct __profile_buf *state)
{
	if(state->index != state->total)
		return 1;
		
	else
	{
#ifdef __PROFILE_IN_WINDOWS
		LARGE_INTEGER now;
		LARGE_INTEGER frequency;
		QueryPerformanceCounter(&now);
		QueryPerformanceFrequency(&frequency);
		state->time = (now.QuadPart - state->timestamp.QuadPart)*1000.0 / frequency.QuadPart;
#else
		struct timeval now;
		gettimeofday(&now, NULL);
		state->time = (now.tv_sec - state->timestamp.tv_sec)*1000.0 + (now.tv_usec - state->timestamp.tv_usec)/1000.0;
#endif
		
		__profileHistory[__profileCurrent] = *state;
		__profileCurrent++;
		__profilePrint(state);
		return 0;
	}
}


static int __profileCompare(const void *p1, const void *p2) {
	struct __profile_buf *b1 = (struct __profile_buf*)p1;
	struct __profile_buf *b2 = (struct __profile_buf*)p2;
	if(b1->time > b2->time)
		return 1;
	else
		return -1;
}


static void __profileSummary()
{
	if(__profileCurrent > 1) {
		
		// GET REFERENCE
		struct __profile_buf reference = __profileHistory[0];
		double base = reference.time/reference.total;
		
		// SORT
		qsort(__profileHistory, __profileCurrent, sizeof(struct __profile_buf), __profileCompare);
		
		// SHOW RESULTS
		printf("\nPROFILER SUMMARY:");
		for(int i = 0; i < __profileCurrent; ++i)
		{
			struct __profile_buf *buf = &__profileHistory[i];
			double time = buf->time/buf->total;
			double factor = base/time;
			
			printf("\n	- %d: [%3.0f%%] \"%s\" ", (i+1), (factor*100), buf->title);
			if(factor == 1.0)
				printf("is the reference.");
			else if(factor > 1.0)
				printf("is %.3f times faster.", factor);
			else
				printf("is %.3f times slower.", (1/factor));
		}
		printf("\n\n");
	}
	__profileCurrent = 0;
}


#ifdef PROFILE_N
#warning PROFILE_N is already defined.
#else

#define PROFILE_N(__TITLE__, __TOTAL__) \
	for(struct __profile_buf buff = __profileInit(__TITLE__, __TOTAL__); \
	__profile(&buff); \
	++buff.index) 
#endif



#ifdef PROFILE
#warning PROFILE is already defined.
#else

#define PROFILE(__TITLE__) PROFILE_N(__TITLE__, 10000000)
#endif



#ifdef PROFILE_SUMMARY
#warning PROFILE_SUMMARY is already defined.
#else

#define PROFILE_SUMMARY() __profileSummary()
#endif

#endif
