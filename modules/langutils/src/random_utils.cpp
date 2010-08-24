#include <langutils/random_utils.h>

#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

namespace fvision {

float ranf() {
	float d = (float) rand() / RAND_MAX;
	return d;
}

float ranf(float min, float max) {
	return min + (max - min) * ranf();
}

double random() {
	double d = (double) rand() / RAND_MAX;
	return d;
}

double random(double min, double max) {
	return min + (max - min) * random();
}

int rani(int max) {
	return (int)(ranf() * max - 0.5);
}

float gaussianRand(float m, float s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2 * ranf() - 1;
			x2 = 2 * ranf() - 1;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1 );

		w = sqrt( (-2 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}

vector<int> ranis(int max, int num) {
	assert(max > 0);
	vector<int> is;
	if (num >= max) {
		for (int i = 0; i < max; i++) {
			is.push_back(i);
		}
	} else {
		for (int i = 0; i < num; ) {
			int v = rani(max);
			if (find(is.begin(), is.end(), v) == is.end()) {
				is.push_back(v);
				++i;
			}
		}
	}
	return is;
}


}