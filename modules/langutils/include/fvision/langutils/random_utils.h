#ifndef FVISION_RANDOM_UTILS_H_
#define FVISION_RANDOM_UTILS_H_

#include <vector>

namespace fvision {

float ranf();

float ranf(float min, float max);

double random();

double random(double min, double max);

int rani(int max);

/* normal random variate generator */
/* mean m, standard deviation s */
float gaussianRand(float m, float s);

//pick num points in range [0, max)
std::vector<int> ranis(int max, int num);

}
#endif // FVISION_RANDOM_UTILS_H_