SMART PROFILER
====
**It's a smart and easy way to profile code in C/C++ and Objective-C. Looks like a native C feature.** Compatible with Windows and Unix-like systems.

##How to use it?

I recommend a short message, for example the name of the function to measure (MESSAGE="sqrt()").

```c
PROFILE( MESSAGE ) {
	// CODE TO PROFILE
}
```

You can specify the number of iterations as well. This time you would have to use PROFILE_N().

```c
PROFILE_N(MESSAGE, iterations) {
	// CODE TO PROFILE
}
```

Finally, you can print a summary comparing the results sorted in a proper way.

```c
void PROFILE_SUMMARY();
```

That's all!!


One more thing, the first call to PROFILE() will be the reference in the summary, ie, if we want to compare two squared root algoritms with the built-in function ```sqrt()```, we should place it first.

``` 
PROFILE("sqrt()")		{ sqrt(n) } //reference
PROFILE("mysqrt()")		{ mysqrt(n) }
PROFILE("mysqrt2()")	{ mysqrt2(n) }
```


##Install it for Xcode 5.0
```
git clone git@github.com:manucorporat/smartProfiler.git
cd smartProfiler
sh install_script.sh
```

##Example code

```c
#include <smartprofiler.h>

static float sqrtApprox(float n)
{    
	// IMPLEMENTATION
}

static float sqrtIterative(float n)
{    
	// IMPLEMENTATION
}

int main()
{
	float v0 = 10000;
    float v1 = 10000;
	float v2 = 10000;

    PROFILE("sqrt()") {
		v2 = sqrt(v2)+13.0f;
	}
    
	PROFILE("sqrtIterative()") {
		v0 = sqrtIterative(v0)+13.0f;
	}  
    
    PROFILE("sqrtApprox()") {
		v1 = sqrtApprox(v1)+13.0f;
	}

	// Prints a summary comparing the results.
    PROFILE_SUMMARY();

    return (v0+v1+v2); // avoid lazy optimations
}
```


OUTPUT:

```
sqrt():
	- Iterations: 10000000
	- Total time: 94.392 ms (0.094392s)
	- Iteration time: 0.00000944 ms (0.000000s)

sqrtIterative():
	- Iterations: 10000000
	- Total time: 489.844 ms (0.489844s)
	- Iteration time: 0.00004898 ms (0.000000s)

sqrtApprox():
	- Iterations: 10000000
	- Total time: 24.706 ms (0.024706s)
	- Iteration time: 0.00000247 ms (0.000000s)


PROFILER SUMMARY:
	- 1: "sqrtApprox()" is 3.821 times faster than "sqrt()".
	- 2: "sqrt()" is the reference.
	- 3: "sqrtIterative()" is 5.189 times slower than "sqrt()".
```