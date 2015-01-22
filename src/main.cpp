#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include "StreetMap.hpp"
#include "Grid.hpp"

using namespace std;

Grid<char> generateStreet(default_random_engine &randomEngine, uniform_real_distribution<double> &uniform01,
                          long segmentCount, double carDensity) {
	Grid<char> street(segmentCount, 1, -1);
	for(auto seg = 0L; seg < street.getXsize(); ++seg) {
		if(uniform01(randomEngine) < carDensity) {
			street(seg, 0) = 3; //TODO randomize speed
		}
	}

	return street;
}

void simulateStreetStep(default_random_engine &randomEngine, uniform_real_distribution<double> &uniform01,
                        Grid<char> &street, double carGenerationRate, double p) {
//	Accellerate
	for(auto seg = 0L; seg < street.getXsize(); ++seg) {
		if(street(seg, 0) != -1 && street(seg, 0) < 5) {
			street(seg, 0) = street(seg, 0) + 1;
		}
	}

//	Slowing down
	for(auto seg = 0L; seg < street.getXsize(); ++seg) {
		if(street(seg, 0) != -1) {
			char speed = street(seg, 0);
			for(auto offset = 1; offset <= speed; ++offset) {
				if(seg + offset >= street.getXsize()) {
					break; // Don't slow down if leaving street
				}
				if(street(seg + offset, 0) != -1) {
					street(seg, 0) = offset - 1;
					break;
				}
			}
		}
	}

//	Randomization
	for(auto seg = 0L; seg < street.getXsize(); ++seg) {
		if(street(seg, 0) > 0) {
			if(uniform01(randomEngine) < p) {
				street(seg, 0) = street(seg, 0) - 1;
			}
		}
	}

//	Car motion
	for(auto seg = 0L; seg < street.getXsize(); ++seg) {
		if(street(seg, 0) != -1) {
			long newSeg = seg + street(seg, 0);
			if(newSeg < street.getXsize()) {
				street(newSeg, 0) = street(seg, 0);
			}
			street(seg, 0) = -1;
		}
	}

//	Create a new car
	if(uniform01(randomEngine) < carGenerationRate && street(0, 0) == -1) {
		street(0, 0) = 3; //TODO random speed
	}
}

void simulateStreet(long segmentCount, double carDensity, double carGenerationRate, double p) {
	default_random_engine randomEngine(chrono::system_clock::now().time_since_epoch().count());
	uniform_real_distribution<double> uniform01 = uniform_real_distribution<double>(0., 1.);

	Grid<char> street = generateStreet(randomEngine, uniform01, segmentCount, carDensity);
	while(true) {
		simulateStreetStep(randomEngine, uniform01, street, p, carGenerationRate);
	}
}

int main(int argc, char **argv) {
	long segmentCount = 300; // 2.25km
	double carDensity = 0.2;
	double carGenerationRate = carDensity;
	double p = 0.3;

	simulateStreet(segmentCount, carDensity, carGenerationRate, p);

	return EXIT_SUCCESS;
}
