#include "CameraReconstructorFactory.h"

#include "CameraReconstructorImpl.h"

using namespace std;

namespace fvision {

CameraReconstructor* CameraReconstructorFactory::create(const Configuration& configuration) {
	switch (configuration.method) {
	case Configuration::CALIBRATED: return new CalibratedCameraReconstructor(configuration.K1.getMat(), configuration.K2.getMat());
	case Configuration::UNCALIBRATED_CANONICAL: return new UnCalibratedCameraReconstructor();
	default: return new UnCalibratedCameraReconstructor();
	}
}

std::ostream& operator<<(std::ostream& os, const CameraReconstructorFactory::Configuration& config) {
	switch (config.method) {
	case CameraReconstructorFactory::Configuration::CALIBRATED: {
		os<<"CALIBRATED";
		os<<"K1: "<<config.K1<<endl;
		os<<"K2: "<<config.K2<<endl;
		break;
									}
	case CameraReconstructorFactory::Configuration::UNCALIBRATED_CANONICAL: {
		os<<"UNCALIBRATED_CANONICAL"; break;
												}
	default: {
		os<<"UNCALIBRATED_CANONICAL";
		break;
			 }
	}
	return os;
}

}