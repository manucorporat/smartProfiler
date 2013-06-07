SMART PROFILER
====
**It's a smart and easy way to profile code in C. Looks like a native C feature.**

##How to use it?

```c
PROFILE("MESSAGE") {
	// CODE TO PROFILE
}
```
or

```c
PROFILE_N("MESSAGE", iterations) {
	// CODE TO PROFILE
}
```

and

```c
PROFILE_SUMMARY(); // to print a summary comparing the results.
}
```

That's all!!


##Example code

```c
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

    PROFILE_SUMMARY();


    return (v0+v1+v2);
}
```


OUTPUT:

```
sqrt():
	- Iterations: 100000000
	- Total time: 2423984 μs  (2.423984 s)
	- Iteration time: 0.024240 μs  (0.000000 s)

sqrtIterative():
	- Iterations: 100000000
	- Total time: 12785562 μs  (12.785562 s)
	- Iteration time: 0.127856 μs  (0.000000 s)

sqrtApprox():
	- Iterations: 100000000
	- Total time: 668946 μs  (0.668946 s)
	- Iteration time: 0.006689 μs  (0.000000 s)


PROFILER SUMMARY:
    - 1: "sqrtApprox()" is 3.624 times faster than "sqrt()"
    - 2: "sqrt()" is the reference.
    - 3: "sqrtIterative()" is 5.275 times slower than "sqrt()"

```