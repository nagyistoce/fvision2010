#include "CameraCalculator.h"

#include <fvision/cvutils.h>

namespace fvision {

double CameraErrorCalculator::compute(const Camera& cam, const ProjectionSample& sample) const {
	CvPoint2D32f bp = cam.project(sample.first);
	return pointDistance(bp, sample.second);
}

}