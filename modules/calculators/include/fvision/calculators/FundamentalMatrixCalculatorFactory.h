#pragma once

#include "FundamentalMatrixCalculator.h"

#include <iosfwd>

namespace fvision {

/** construct a FundamentalMatrixCalculator 

*/
class FundamentalMatrixCalculatorFactory {
public:
	class Configuration {
	public:
		Configuration() {
			method = RANSAC;
			normalize = true;
			essential = false;
			ransacDistanceThreshold = 0.0015;
			ransacModelSampleSize = 8;
		}

		bool isValid() const {
			return (ransacDistanceThreshold >0 && ransacModelSampleSize >= 8);
		}

	public:
		enum Method {
			DLT,
			RANSAC,
			HOMOGRAPHY_BASED,
			ASSUME_TRANSLATION
		};

	public:
		Method method;
		bool normalize;
		bool essential; // if essential is set, normalize is neglected, assume the input is already normalized by K

		double ransacDistanceThreshold;
		int ransacModelSampleSize;
	};

public:
	FundamentalMatrixCalculator* create(const Configuration& configuration);
};

std::ostream& operator<<(std::ostream& os, const FundamentalMatrixCalculatorFactory::Configuration& config);

}