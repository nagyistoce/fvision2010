#pragma once

#include "CameraReconstructor.h"

namespace fvision {

class CalibratedCameraReconstructor : public CameraReconstructor {
public:
	CalibratedCameraReconstructor(const CvMat* K1, const CvMat* K2);
	~CalibratedCameraReconstructor(void);

public:
	void compute(const CvMat* F, const PointPairs& pps);

private:
	CvMat* K1;
	CvMat* K2;

	std::vector<CvPoint2D32f> x1s;
	std::vector<CvPoint2D32f> x2s;
};

class UnCalibratedCameraReconstructor : public CameraReconstructor {
public:
	UnCalibratedCameraReconstructor();
	~UnCalibratedCameraReconstructor(void);

public:
	void compute(const CvMat* F, const PointPairs& pps);

};

}