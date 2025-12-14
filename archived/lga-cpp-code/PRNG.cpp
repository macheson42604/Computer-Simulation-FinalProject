#include <algorithm>
#include <cmath>

#include "PRNG.h"

using namespace std;

double PRNG::uniform(double a, double b) {
	return a + (b - a) * random();
}

long PRNG::equilikely(long a, long b) {
	return a + static_cast<long>((b - a + 1) * random());
}

double PRNG::exponential(double mu) {
	return -mu * log(1 - random());
}

long PRNG::geometric(double p) {
	return static_cast<long>(log(1 - random()) / log(p));
}