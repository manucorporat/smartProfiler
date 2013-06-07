
#include <stdio.h>
#include <math.h>
#include "profiler.h"


// Babylonian method: http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Babylonian_method
static float sqrtNewton(float n) {
    
	float x = n;
    float x1, diff;
    do {
        x1 = (x + n/x)/2;
        diff = x-x1;
		x = x1;
        
    }while(diff!=0.0f);
    
	return x;
}


int main()
{
	float v0 = 1000;
	PROFILE("ALGORITHM 1, Newton's method") {
		v0 = sqrtNewton(v0)+13.0f;
	}
    
	float v1 = 1000;
	PROFILE("ALGORITHM 2, Native sqrt() function") {
		v1 = sqrt(v1)+25.0f;
	}
    
    return (v0+v1); // prevent lazy optimizations
}
