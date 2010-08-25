#pragma once

#include "TwoViewReconstructor.h"
#include "CameraReconstructorFactory.h"

#include <fvision/calculators.h>
#include <iostream>

namespace fvision {

class TwoViewReconstructorFactory {
public:
	typedef FundamentalMatrixCalculatorFactory::Configuration FConfiguration;
	typedef CameraReconstructorFactory::Configuration CamConfiguration;

	class Configuration {
	public:
		Configuration() {
			thresholdOfReprojectionError = 2.0;
			directEssential = false;
		}
	public:
		FConfiguration fConfiguration;
		CamConfiguration camConfiguration;
		double thresholdOfReprojectionError; //used to seperate inliers and outliers based on reprojection error
		bool directEssential; //direct compute essential matrix
	};

public:
	TwoViewReconstructor* create(const Configuration& configuration);
};

std::ostream& operator<<(std::ostream& os, const TwoViewReconstructorFactory::Configuration& config);

}