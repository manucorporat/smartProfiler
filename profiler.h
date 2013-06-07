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



typedef struct _profile_buf {
	const char *title;
    long long total;
    long long index;
    double time;
#ifdef __PROFILE_IN_WINDOWS
    LARGE_INTEGER timestamp;
#else
	struct timeval timestamp;
#endif
} profile_buf;


profile_buf __profileHistory[20];
int __profileCurrent = 0;


inline profile_buf __initProfile(const char *title, long long total)
{
	profile_buf state;
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


void __profilePrint(profile_buf *state)
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


inline char __profile(profile_buf *state)
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


int __profileCompare(const void *p1, const void *p2)
{
    profile_buf *b1 = (profile_buf*)p1;
    profile_buf *b2 = (profile_buf*)p2;
    return (int)(b1->time - b2->time);
}


void PROFILE_SUMMARY()
{
    if(__profileCurrent > 1) {
        
        // GET REFERENCE
        profile_buf reference = __profileHistory[0];
        double base = reference.time/reference.total;
        
        // SORT
        qsort(&__profileHistory, __profileCurrent, sizeof(profile_buf), __profileCompare);
        
        // SHOW RESULTS
        printf("\nPROFILER SUMMARY:");
        for(int i = 0; i < __profileCurrent; ++i)
        {
            profile_buf *buf = &__profileHistory[i];
            double time = buf->time/buf->total;
            double factor = base/time;
            
            printf("\n	- %d: \"%s\" ", (i+1), buf->title);
            if(factor == 1.0f)
                printf("is the reference.");
            else if(factor >= 1)
                printf("is %.3f times faster than \"%s\".", factor, reference.title);
            else
                printf("is %.3f times slower than \"%s\".", (1.0f/factor), reference.title);
        }
        printf("\n\n");
    }
    __profileCurrent = 0;
}


#define PROFILE_N(__TITLE__, __TOTAL__) \
for(profile_buf buff = __initProfile(__TITLE__, __TOTAL__); __profile(&buff); ++buff.index)

#define PROFILE(__TITLE__) PROFILE_N(__TITLE__, 10000000)


#endif