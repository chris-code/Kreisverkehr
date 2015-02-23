#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include "StreetMap.hpp"
#include "Grid.hpp"
#include "CImg.h"

using namespace std;
namespace cn = cimg_library;

cn::CImg<unsigned char> gridToImg(Grid<char>& grid) {
	cn::CImg<unsigned char> img(grid.getXsize(), grid.getYsize());

	for (auto x = 0L; x < grid.getXsize(); ++x) {
		for (auto y = 0L; y < grid.getYsize(); ++y) {
			img(x, y) = grid(x, y) + 1;
		}
	}

	return img;
}


cn::CImg<unsigned char> append(cn::CImg<unsigned char>& bigImg, cn::CImg<unsigned char>& smallImg) {
	return bigImg.get_append(smallImg, 'y', 0);
}


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
                        Grid<char> &street, double p) {
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
				if(street((seg + offset + street.getXsize()) % street.getXsize(), 0) != -1) {
					street(seg, 0) = offset - 1;
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
			long newSeg = (seg + street(seg, 0) + street.getXsize()) % street.getXsize();
			street(newSeg, 0) = street(seg, 0);
			street(seg, 0) = -1;
		}
	}
}

void simulateStreet(long segmentCount, double carDensity, double p) {
	default_random_engine randomEngine(chrono::system_clock::now().time_since_epoch().count());
	uniform_real_distribution<double> uniform01 = uniform_real_distribution<double>(0., 1.);

	Grid<char> street = generateStreet(randomEngine, uniform01, segmentCount, carDensity);
	cn::CImg<unsigned char> streetImg = gridToImg(street);
	cn::CImg<unsigned char> whiteImg(street.getXsize(), 50, 1, 1, 255);

	streetImg = append(streetImg, whiteImg);

	cout << streetImg.height() << endl;

	cn::CImgDisplay initDisplay(streetImg, "Street", 1);

	while(true) {
		simulateStreetStep(randomEngine, uniform01, street, p);
	}
}

int main(int argc, char **argv) {
	long segmentCount = 300; // 2.25km
	double carDensity = 0.2;
	double p = 0.3;

	simulateStreet(segmentCount, carDensity, p);

	return EXIT_SUCCESS;
}
