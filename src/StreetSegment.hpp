#include <random>

using namespace std;

enum class Direction {UP, RIGHT, DOWN, LEFT};

class StreetSegment {
	public:
		StreetSegment(default_random_engine& randomEngine, double up = 0.25, double right = 0.25, double down = 0.25, double left = 0.25) : randomEngine(randomEngine)
		{
			this->up = up;
			this->right = right;
			this->down = down;
			this->left = left;
		}

		Direction getDirection()
		{
			double randNum = distribution(randomEngine);
			double cumProbs = up;

			if (randNum < cumProbs) {
				return Direction::UP;
			}
			else {
				cumProbs += right;
				if (randNum < cumProbs) {
					return Direction::RIGHT;
				}
				else {
					cumProbs += down;
					if (randNum < cumProbs) {
						return Direction::DOWN;
					}
					else {
						return Direction::LEFT;
					}
				}
			}
		}

	private:
		double up, right, down, left;
		default_random_engine& randomEngine;
		static uniform_real_distribution<double> distribution;

};

uniform_real_distribution<double> StreetSegment::distribution = uniform_real_distribution<double>(0.,1.);
