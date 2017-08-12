#ifndef RANDOM
#define RANDOM

#include<iostream>
using namespace std;
class random
{
	static const long long M = ((((long long)1) << 31) - 1);
	static const long long A = 48271;
	long long x;

public:
	random(int x = 1)
	{
		this->x = (long long)x;
	}

	int next()
	{
		x = (A*x) % M;
		return (int)(x - 1);
	}

	int next(int maxValue)
	{
		x = (A*x) % M;
		return (int)((x - 1) % maxValue);
	}

	int next(int minValue, int maxValue)
	{
		x = (A*x) % M;
		return (minValue + ((x - 1) % (maxValue - minValue + 1)));
	}

	double nextDouble()
	{
		x = (A*x) % M;
		return ((double)(x - 1)) / (M - 1);
	}
};
#endif
