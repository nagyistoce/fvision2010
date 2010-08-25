#include "TwoViewReconstructorFactory.h"

#include "TwoViewReconstructorImpl.h"
#include <fvision/cvutils.h>

using namespace std;

namespace fvision {

TwoViewReconstructor* TwoViewReconstructorFactory::create(const Configuration& configuration) {
	if (configuration.directEssential) {
		return new CalibratedTwoViewReconstructorImpl(
			configuration.camConfiguration.K1.getMat(), 
			configuration.camConfiguration.K2.getMat(), 
			configuration.fConfiguration.ransacDistanceThreshold / abs(cvmGet(configuration.camConfiguration.K1.getMat(), 0, 0)), 
			configuration.thresholdOfReprojectionError);
	} else {
		return new TwoViewReconstructorImpl(
			FundamentalMatrixCalculatorFactory().create(configuration.fConfiguration),
			CameraReconstructorFactory().create(configuration.camConfiguration),
			new PointReconstructor(configuration.thresholdOfReprojectionError));
	}
}

std::ostream& operator<<(std::ostream& os, const TwoViewReconstructorFactory::Configuration& config) {
	os<<"direct essential: "<<config.directEssential<<endl;
	os<<"thresholdOfReprojectionError: "<<config.thresholdOfReprojectionError<<endl;
	if (!config.directEssential) {
		os<<"f config: "<<endl;
		os<<config.fConfiguration<<endl;
		os<<"camera config: "<<endl;
		os<<config.camConfiguration<<endl;
	} else {
		cout<<"ransac distance threshold: "<<config.fConfiguration.ransacDistanceThreshold<<endl;
		cout<<"K: "<<config.camConfiguration.K1<<endl;
	}
	return os;
}
}