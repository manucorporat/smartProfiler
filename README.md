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

That's all!!


##Example code

```c
static float sqrtNewton(float n)
{    
	float x = n;
	float x1;
	float diff;
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
	float v1 = 1000;


	PROFILE("ALGORITHM 1, Newton's method") {
		v0 = sqrtNewton(v0)+13.0f;
	}

	PROFILE("ALGORITHM 2, Native sqrt() function") {
		v1 = sqrt(v1)+13.0f;
	}

    return (v0+v1); // prevent lazy optimizations
}
```

OUTPUT:

```
ALGORITHM 1, Newton's method:
	Iterations: 100000000
	Total time: 4882027 μs  (4.882027 s)
	Iteration time: 0.048820 μs  (0.000000 s)
	
ALGORITHM 2, Native sqrt() function:
	- Iterations: 100000000
	- Total time: 922398 μs  (0.922398 s)
	- Iteration time: 0.009224 μs  (0.000000 s)
```