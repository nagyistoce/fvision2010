#pragma once

#include <fvision/geom.h>

namespace fvision {

// reconstruct camera from the fundamental matrix and point correspondences
class CameraReconstructor {
public:
	virtual ~CameraReconstructor() {}
	virtual void compute(const CvMat* F, const PointPairs& pps) = 0;
	const Camera& getCamera1() const { return cam1; }
	const Camera& getCamera2() const { return cam2; }

protected:
	Camera cam1;
	Camera cam2;
};




}