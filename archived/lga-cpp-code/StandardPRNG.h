#ifndef STANDARD_PRNG_H
#define STANDARD_PRNG_H

#include <random>

#include "PRNG.h"

class StandardPRNG : public PRNG {
public:
	StandardPRNG(unsigned int seed);
	
	double random() override;
private:
	std::mt19937 rng;
	std::uniform_real_distribution<double> generator;
};

#endif