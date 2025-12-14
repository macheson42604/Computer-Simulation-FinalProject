#include <random>

#include "StandardPRNG.h"

#include "PRNG.h"

StandardPRNG::StandardPRNG(unsigned int seed) {
	rng = std::mt19937(seed);
	generator = std::uniform_real_distribution<double>(0.0, 1.0);
}
	
double StandardPRNG::random() {
	double result = 0.0;
	
	while (result == 0.0) {
		result = generator(rng);
	}
	
	return result;
}