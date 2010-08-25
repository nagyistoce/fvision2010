#pragma once

#include "CameraReconstructor.h"
#include <fvision/cvutils.h>

#include <iostream>

namespace fvision {

class CameraReconstructorFactory {
public:
	class Configuration {
	public:
		enum Method {
			CALIBRATED,
			UNCALIBRATED_CANONICAL,
			UNCALIBRATED_ALLPOINTS
		};
	public:
		Method method;
		CvMatWrapper K1;
		CvMatWrapper K2;
	};

public:
	CameraReconstructor* create(const Configuration& configuration);
};

std::ostream& operator<<(std::ostream& os, const CameraReconstructorFactory::Configuration& config);

}
