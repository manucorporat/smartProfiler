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
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>



typedef struct _profile_buf {
	const char *title;
	unsigned long long total;
	unsigned long long index;
    unsigned long long time;
	struct timeval timestamp;
} profile_buf;


profile_buf __profileHistory[20];
unsigned int __profileCurrent = 0;


inline profile_buf __initProfile(const char *title, unsigned long long total)
{
	profile_buf state;
	state.title = title;
	state.total = total;
	state.index = 0;
	gettimeofday(&state.timestamp, NULL);
	return state;
}


void __profilePrint(profile_buf *state)
{
    double iterationTime = (double)state->time/(double)state->total;
    
    printf(
           "%s:\n"
           "	- Iterations: %llu\n"
           "	- Total time: %llu μs  (%f s)\n"
           "	- Iteration time: %f μs  (%f s)\n\n",
           state->title,
           state->total,
           state->time, (state->time/1000000.0),
           iterationTime, (iterationTime/1000000.0));
}


inline bool __profile(profile_buf *state)
{
	if(state->index != state->total)
		return true;
		
	else
	{
		struct timeval now;
		gettimeofday(&now, NULL);
        state->time = (now.tv_sec - state->timestamp.tv_sec)*1000000 + (now.tv_usec - state->timestamp.tv_usec);

        __profileHistory[__profileCurrent] = *state;
        __profileCurrent++;
        __profilePrint(state);
		return false;
	}
}


int __profileCompare(const void *p1, const void *p2) {
    profile_buf *b1 = (profile_buf*)p1;
    profile_buf *b2 = (profile_buf*)p2;
    return b1->time - b2->time;
}

void PROFILE_SUMMARY()
{
    if(__profileCurrent > 1) {
        
        // GET REFERENCE
        profile_buf reference = __profileHistory[0];
        double base = (double)reference.time/(double)reference.total;
        
        // SORT
        qsort(&__profileHistory, __profileCurrent, sizeof(profile_buf), __profileCompare);
        
        // SHOW RESULTS
        int i = 0;
        printf("\nPROFILER SUMMARY:");
        for(; i < __profileCurrent; ++i) {
            profile_buf *buf = &__profileHistory[i];
            double time = (double)buf->time/(double)buf->total;
            double relation = base / time;
            
            printf("\n    - %d: \"%s\" ", (i+1), buf->title);
            if(relation == 1.0f)
                printf("is the reference.");
            else if(relation >= 1)
                printf("is %.3f times faster than \"%s\"", relation, reference.title);
            else
                printf("is %.3f times slower than \"%s\"", (1.0f/relation), reference.title);
        }
        printf("\n\n");
        
    }
    __profileCurrent = 0;
}


#define PROFILE_N(__TEXT__, __TOTAL__) \
for(profile_buf buff = __initProfile(__TEXT__, __TOTAL__); __profile(&buff); ++buff.index)

#define PROFILE(__TEXT__) PROFILE_N(__TEXT__, 100000000)


#endif