#include "TwoViewReconstructorImpl.h"

#include <fvision/geom.h>
#include <fvision/calculators.h>

#include <cassert>

using namespace std;

namespace fvision {

TwoViewReconstructorImpl::TwoViewReconstructorImpl(
	FundamentalMatrixCalculator* fmc,
	CameraReconstructor* cameraReconstructor,
	PointReconstructor* pointReconstructor) 
{
	assert(fmc != NULL && cameraReconstructor != NULL && pointReconstructor != NULL);
	this->fmc = fmc;
	this->cameraReconstructor = cameraReconstructor;
	this->pointReconstructor = pointReconstructor;
}

TwoViewReconstructorImpl::~TwoViewReconstructorImpl(void) {
	delete fmc;
	delete cameraReconstructor;
	delete pointReconstructor;
}

bool TwoViewReconstructorImpl::compute(const PointPairs& pps) {

	CvMatWrapper fw = fmc->compute(pps);

	results.fInlierIndices = fmc->getInlierIndices();
	results.fOutlierIndices = fmc->getOutlierIndices();

	cameraReconstructor->compute(fw.getMat(), pps);

	Camera leftCamera = cameraReconstructor->getCamera1();
	Camera rightCamera = cameraReconstructor->getCamera2();

	vector<CvPoint2D32f> x1s, x2s;
	decomposePointPairs(pps, x1s, x2s);
	pointReconstructor->compute(leftCamera.getP(), rightCamera.getP(), x1s, x2s);

	results.fw = fw;
	results.cameras.resize(2);
	results.cameras[0] = leftCamera;
	results.cameras[1] = rightCamera;
	results.pointResults = pointReconstructor->results;

	return true;
}

CalibratedTwoViewReconstructorImpl::CalibratedTwoViewReconstructorImpl(
	const CvMat* K1, 
	const CvMat* K2, 
	double thresholdOfEssentialMatrixCalculation, 
	double thresholdOfReprojectionError) 
{
	this->K1 = K1;
	this->K2 = K2;

	this->thresholdOfReprojectionError = thresholdOfReprojectionError;
	this->thresholdOfEssentialMatrixCalculation = thresholdOfEssentialMatrixCalculation;

	this->inputIsNormalized = false;
	this->assumeTranslation = false;
}

CalibratedTwoViewReconstructorImpl::~CalibratedTwoViewReconstructorImpl(void) {
}

bool CalibratedTwoViewReconstructorImpl::compute(const PointPairs& pps) {

	vector<CvPoint2D32f> x1s, x2s;
	decomposePointPairs(pps, x1s, x2s);

	vector<CvPoint2D32f> nx1s = x1s;
	vector<CvPoint2D32f> nx2s = x2s;

	if (!inputIsNormalized) {
		nx1s = normalizePointsWithCameraInternalMatrix(K1.getMat(), x1s);
		nx2s = normalizePointsWithCameraInternalMatrix(K2.getMat(), x2s);
	}

	computeCameras(nx1s, nx2s);
	computePoints(x1s, x2s);

	return true;
}

void CalibratedTwoViewReconstructorImpl::computeCameras(const std::vector<CvPoint2D32f>& nx1s, const std::vector<CvPoint2D32f>& nx2s) {

	PointPairs npps;
	composePointPairs(npps, nx1s, nx2s);

	//compute essential matrix from normalized point correspondences
	FundamentalMatrixCalculatorFactory factory;
	FundamentalMatrixCalculatorFactory::Configuration config;
	config.ransacDistanceThreshold = thresholdOfEssentialMatrixCalculation;
	if (assumeTranslation) {
		config.method = FundamentalMatrixCalculatorFactory::Configuration::ASSUME_TRANSLATION;
	}
	FundamentalMatrixCalculator* fmc = factory.create(config);
	//HomographyBasedFundamentalMatrixCalculator* hfmc = new HomographyBasedFundamentalMatrixCalculator(thresholdOfEssentialMatrixCalculation);
	//hfmc->setEssential(true);
	//fmc = hfmc;

	CvMatWrapper ew = fmc->compute(npps);

	results.fInlierIndices = fmc->getInlierIndices();
	results.fOutlierIndices = fmc->getOutlierIndices();

	delete fmc;

	Camera leftCamera;
	CvMat* I = getIdentity();
	Camera rightCamera = getCameraFromEssentialMatrix(ew.getMat(), nx1s, nx2s, I);
	cvReleaseMat(&I);

	if (!inputIsNormalized) {
		leftCamera = Camera(K1.getMat());
		rightCamera = Camera(K2.getMat(), rightCamera.getR(), rightCamera.gett());
	}

	results.fw = ew;
	if (!inputIsNormalized) {
		results.fw = getFFromEK(ew.getMat(), K1.getMat(), K2.getMat());
	}
	results.cameras.resize(2);
	results.cameras[0] = leftCamera;
	results.cameras[1] = rightCamera;
}

Camera CalibratedTwoViewReconstructorImpl::getCameraFromEssentialMatrix(const CvMat* E, const std::vector<CvPoint2D32f>& nx1s, const std::vector<CvPoint2D32f>& nx2s, const CvMat* K) {

	//compute R, t
	CvMat* R = cvCreateMat(3, 3, CV_64FC1);
	CvMat* t = cvCreateMat(3, 1, CV_64FC1);

	computeRt(E, nx1s, nx2s, R, t);

	Camera cam(K, R, t);

	cvReleaseMat(&R);
	cvReleaseMat(&t);

	return cam;
}

void CalibratedTwoViewReconstructorImpl::computePoints(const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) {

	PointReconstructor* pointReconstructor = new PointReconstructor(thresholdOfReprojectionError);
	pointReconstructor->compute(results.cameras[0].getP(), results.cameras[1].getP(), x1s, x2s);

	results.pointResults = pointReconstructor->results;

	delete pointReconstructor;
}

}