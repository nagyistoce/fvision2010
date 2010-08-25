#pragma once

#include "HomographyCalculator.h"

namespace fvision {

class HomographyCalculatorFactory {
public:
	class Configuration {		
	public:
		Configuration() {
			method = RANSAC;
			normalize = true;
			ransacDistanceThreshold = 0.0015;
			ransacModelSampleSize = 5;
		}

		bool isValid() const {
			return (ransacDistanceThreshold >0 && ransacModelSampleSize >= 4);
		}

	public:
		enum Method {
			DLT,
			RANSAC
		};

	public:
		Method method;
		bool normalize;
		double ransacDistanceThreshold;
		int ransacModelSampleSize;
	};
public:
	HomographyCalculator* create(const Configuration& configuration);
	HomographyCalculator* createDLT();
	/// normalization is used
	HomographyCalculator* createRansac(double threshold);
};

std::ostream& operator<<(std::ostream& os, const HomographyCalculatorFactory::Configuration& config);

}