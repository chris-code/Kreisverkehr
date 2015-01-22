#include <cstdlib>
#include <iostream>
#include <random>
#include <chrono>
#include "StreetSegment.hpp"

using namespace std;

int main(int argc, char **argv) {
	default_random_engine randomEngine(chrono::system_clock::now().time_since_epoch().count());

	StreetSegment ss(randomEngine, 0.3, 0.3, 0.2, 0.2);
	Direction d = ss.getDirection();

	cout << int(d) << endl;

	return EXIT_SUCCESS;
}
