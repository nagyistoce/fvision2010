#include "HomographyCalculator.h"

namespace fvision {

HomographyGeometricErrorCalculator::HomographyGeometricErrorCalculator(int metric) {
	assert(metric == 1 || metric == 2);

	this->metric = metric; 
}

double HomographyGeometricErrorCalculator::compute(const CvMatWrapper& fw, const PointPair& sample) const {

	CvPoint2D32f hp1 = transformPoint(fw.getMat(), sample.first);
	return pointDistance(hp1, sample.second, metric);
}


}