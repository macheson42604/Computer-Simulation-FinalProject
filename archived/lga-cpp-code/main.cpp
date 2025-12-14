#include <fstream>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>

#include "StandardPRNG.h"

struct Point {
	double x, y;
};

int main(int argc, char* argv[]) {
	if (argc < 4) {
		std::cerr << "Program requires a file name, seed, and number of points to generate as an argument" << '\n';
		exit(1);
	}
	
	std::ifstream fin(argv[1]);
	if (fin.fail()) {
		std::cerr << "Failed to open file" << '\n';
		exit(1);
	}
	
	StandardPRNG prng(std::stoi(argv[2]));
	int N = std::stoi(argv[3]);
	
	std::vector<Point> data;
	double x, y;
	while (fin >> x >> y) {
		data.push_back({x, y});
	}
	fin.close();
	
	if (data.size() < 2) {
		std::cerr << "Not enough data in file" << '\n';
		exit(1);
	}
	
	std::vector<double> cdfValues = {0};
	cdfValues.reserve(data.size());
	double totalArea = 0;
	for (unsigned int i = 1; i < data.size(); i++) {
		double area = 0.5 * (data.at(i).x - data.at(i - 1).x) * (data.at(i).y + data.at(i - 1).y);
		totalArea += area;
		cdfValues.push_back(cdfValues.back() + area);
	}
	
	for (unsigned int i = 0; i < data.size(); i++) {
		data.at(i).y /= totalArea;
		cdfValues.at(i) /= totalArea;
	}
	
	std::vector<double> As, Bs, Cs;
	As.reserve(data.size() - 1);
	Bs.reserve(data.size() - 1);
	Cs.reserve(data.size() - 1);
	for (unsigned int i = 1; i < data.size(); i++) {
		double slope = (data.at(i).y - data.at(i - 1).y) / (data.at(i).x - data.at(i - 1).x);
		As.push_back(slope / 2);
		Bs.push_back(data.at(i - 1).y - slope * data.at(i - 1).x);
		Cs.push_back(((slope / 2) * data.at(i - 1).x - data.at(i - 1).y) * data.at(i - 1).x);
	}
	
	std::cout << std::fixed << std::setprecision(17);
	for (int i = 0; i < N; i++) {
		double u = prng.random();
		int index = std::distance(cdfValues.begin(), std::upper_bound(cdfValues.begin(), cdfValues.end(), u)) - 1;
		
		double a = As.at(index), b = Bs.at(index), c = Cs.at(index) - u + cdfValues.at(index);
		double x = (-b + std::sqrt(std::pow(b, 2) - 4 * a * c)) / (2 * a);
		
		std::cout << x << '\n';
	}
	
	return 0;
}