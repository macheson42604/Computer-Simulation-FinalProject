#ifndef PRNG_H
#define PRNG_H

class PRNG {
public:
	virtual ~PRNG() {};
	
	virtual double random() = 0;
	
	double uniform(double a, double b);
	
	long equilikely(long a, long b);
	
	double exponential(double mu);
	
	long geometric(double p);
	
	template <class Iterator>
	void shuffle(Iterator start, Iterator end);
};

template <class Iterator>
void PRNG::shuffle(Iterator start, Iterator end) {
	if (start == end) {
		return;
	}
	
	unsigned int n = end - start;
	for (unsigned int c = 0; c < n - 1; c++) {
		std::swap(*(start + c), *(start + equilikely(c, n - 1)));
	}
}

#endif