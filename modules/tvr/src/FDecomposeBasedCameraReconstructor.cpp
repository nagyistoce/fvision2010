#include "FDecomposeBasedCameraReconstructor.h"

#include "CameraReconstructorImpl.h"
#include "FDecomposer.h"

#include <fvision/geom.h>
#include <iostream>

using namespace std;

namespace fvision {

FDecomposeBasedCameraReconstructor::FDecomposeBasedCameraReconstructor(const CvPoint2D32f& principalPoint) {
	this->principalPoint = principalPoint;
}

FDecomposeBasedCameraReconstructor::~FDecomposeBasedCameraReconstructor(void)
{
}

void FDecomposeBasedCameraReconstructor::compute(const CvMat* F, const PointPairs& pps) {
	//first transform F to nF
	//K' = kH * K
	//kH = [1, 0, -px; 0, 1, -py; 0, 0, 1] assuming the upleft is origin
	double kh[] = {
		1,  0, -principalPoint.x,
		0,  -1, principalPoint.y,
		0,  0,   1};
	CvMat kH = cvMat(3, 3, CV_64FC1, kh);

	CvMat* nF = transF(F, &kH, &kH);

	FDecomposer fd;
	FParams fp = fd.compute(nF);

	if (fd.getStatus() != FDecomposer::SUCCESS) {
		cout<<"fail to decompse!"<<endl;
		UnCalibratedCameraReconstructor ucr;
		ucr.compute(F, pps);
		cam1 = ucr.getCamera1();
		cam2 = ucr.getCamera2();
	} else {
		FComposer fc(fp);
		CvMat* K1 = matMul(&kH, fc.K1);
		CvMat* K2 = matMul(&kH, fc.K2);
		cam1.reset(K1, fc.R1, fc.t1);
		cam2.reset(K2, fc.R2, fc.t2);
		clearMats(&K1, &K2);
		cout<<"params: "<<fp<<endl;
		cout<<"K1: "<<K1<<endl;
		cout<<"K2: "<<K2<<endl;
	}

	cvReleaseMat(&nF);
}
}