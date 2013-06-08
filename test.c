
#include <stdio.h>
#include <math.h>
#include "profiler.h"


// Babylonian method: http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Babylonian_method
// x(i+1) = x(i)/2 + (S/x(i))/2

static float sqrtIterative(float n) {
	
	register float x = n;
	register float x1, diff;
	do {
		x1 = (x + n/x)/2;
		diff = x-x1;
		x = x1;
		
	}while(diff!=0.0f);
	
	return x;
}


static float sqrtApprox(float z)
{
	int val_int = *(int*)&z; /* Same bits, but as an int */
	
	val_int -= 1 << 23; /* Subtract 2^m. */
	val_int >>= 1; /* Divide by 2. */
	val_int += 1 << 29; /* Add ((b + 1) / 2) * 2^m. */
	
	return *(float*)&val_int; /* Interpret again as float */
}


static float sqrtBakhsali(const float m)
{
	int i=0;
	while( (i*i) <= m )
		i++;
	i--;
	float d = m - i*i;
	float p=d/(2*i);
	float a=i+p;
	return a-(p*p)/(2*a);
}


int main()
{
	float v0 = 10000;
	float v1 = 10000;
	float v2 = 10000;
	float v3 = 10000;


	PROFILE("sqrt()") {
		v0 = sqrt(v0)+13.0f;
	}
	
	PROFILE("sqrtIterative()") {
		v1 = sqrtIterative(v1)+13.0f;
	}
	
	PROFILE("sqrtBakhsali()") {
		v2 = sqrtBakhsali(v2)+13.0f;
	}
	
	PROFILE("sqrtApprox()") {
		v3 = sqrtApprox(v3)+13.0f;
	}

	PROFILE_SUMMARY();
	
	
	return (v0+v1+v2+v3); // avoid lazy optimations
}
