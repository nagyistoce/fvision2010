#include "CameraReconstructorImpl.h"

#include "FDecomposer.h"

#include <fvision/geom.h>

using namespace std;

namespace fvision {

CalibratedCameraReconstructor::CalibratedCameraReconstructor(const CvMat* K1, const CvMat* K2) {
	this->K1 = cloneMat(K1);
	this->K2 = cloneMat(K2);

	CvMat* I = getIdentity();
	CvMat* Zero = getZero(3, 1);

	cam1 = Camera(K1, I, Zero);

	cvReleaseMat(&I);
	cvReleaseMat(&Zero);
}

CalibratedCameraReconstructor::~CalibratedCameraReconstructor(void) {
	cvReleaseMat(&K1);
	cvReleaseMat(&K2);
}

void doFDecompose(const CvMat* F, const CvPoint2D32f& pp) {
	double kh[] = {
		1,  0, -pp.x,
		0,  -1, pp.y,
		0,  0,   1};

		CvMat kH = cvMat(3, 3, CV_64FC1, kh);

		CvMat* nF = transF(F, &kH, &kH);

		FDecomposer fd;
		FParams fp = fd.compute(nF);

		if (fd.getStatus() != FDecomposer::SUCCESS) {
			cout<<"failed in F decomposition!"<<endl;
		} else {
			cout<<"params: "<<fp<<endl;
		}

		cout<<"e1: "<<null(F)<<endl;
		cout<<"e2: "<<leftNull(F)<<endl;
}

void CalibratedCameraReconstructor::compute(const CvMat* F, const PointPairs& pps) {

	CvMat* E = getEFromFK(F, K1, K2);

	//compute R, t
	CvMat* R = cvCreateMat(3, 3, CV_64FC1);
	CvMat* t = cvCreateMat(3, 1, CV_64FC1);

	vector<CvPoint2D32f> x1s, x2s;
	decomposePointPairs(pps, x1s, x2s);

	vector<CvPoint2D32f> nx1s = normalizePointsWithCameraInternalMatrix(K1, x1s);
	vector<CvPoint2D32f> nx2s = normalizePointsWithCameraInternalMatrix(K2, x2s);

	computeRt(E, nx1s, nx2s, R, t);

	cam2.reset(K2, R, t);
	cout<<"R: "<<R<<endl;
	//cam2.reset(K2, getIdentity(), matMul(inv(K2), leftNull(F)));

	clearMats(&E, &R, &t);

	CvPoint2D32f pp = cvPoint2D32f(cvmGet(K1, 0, 2), cvmGet(K1, 1, 2));
	doFDecompose(F, pp);

}

UnCalibratedCameraReconstructor::UnCalibratedCameraReconstructor() {
	CvMat* I = getIdentity();
	CvMat* Zero = getZero(3, 1);
	cam1 = Camera(I, I, Zero);
	cvReleaseMat(&I);
	cvReleaseMat(&Zero);
}

//M = [e']xF, m = e'
void UnCalibratedCameraReconstructor::compute(const CvMat* F, const PointPairs& pps) {
	CvMat* e2 = leftNull(F);
	CvMat* ce2 = getCrossMatrix(e2);
	CvMat* M = matMul(ce2, F);
	CvMat* I = getIdentity();
	cam2.reset(I, M, e2);
	clearMats(&e2, &ce2, &M, &I);
}

UnCalibratedCameraReconstructor::~UnCalibratedCameraReconstructor(void) {

}

}