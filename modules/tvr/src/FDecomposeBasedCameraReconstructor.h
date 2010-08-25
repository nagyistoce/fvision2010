#pragma once

#include "CameraReconstructor.h"

namespace fvision {

class FDecomposeBasedCameraReconstructor  : public CameraReconstructor {
public:
	FDecomposeBasedCameraReconstructor(const CvPoint2D32f& principalPoint);
	~FDecomposeBasedCameraReconstructor(void);

public:
	void compute(const CvMat* F, const PointPairs& pps);

private:
	CvPoint2D32f principalPoint;
};

}