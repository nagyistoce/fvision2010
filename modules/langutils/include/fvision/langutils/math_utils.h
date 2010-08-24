#ifndef FVISION_MATH_H_
#define FVISION_MATH_H_

#include <vector>

/*!
math functions
mainly probability functions, random sample

*/
namespace fvision {

	extern double PI;

	double degreesToRadian(double degrees);
	double degreesFromRadian(double radian);
	double radianToDegrees(double radian);

	void doMeanCovStatistics(const std::vector<double>& vs, double& mean, double& scov);
	double getMean(const std::vector<double>& vs);
	double getAbsMean(const std::vector<double>& vs);

	float getMean(const std::vector<float>& vs);

	// normalize theta so that it is in [-pi/2, pi/2]
	double normalizeAngleToHalfPi(double theta);

	// normalize theta so that it is in [-pi, pi]
	double normalizeAngleToPi(double theta);
}
#endif // FVISION_MATH_H_