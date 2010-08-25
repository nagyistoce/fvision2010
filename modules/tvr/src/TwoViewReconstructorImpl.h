#pragma once

#include "TwoViewReconstructor.h"
#include "CameraReconstructor.h"

#include <fvision/calculators.h>

namespace fvision {

/**
 * use normalized ransac algorithm to compute F
 *    user need to input a threshold for ransac algorithm
 * for camera reconstruction: 
 *    user either calibrated reconstruction method, which requires user to input calibration information on both image
 *    or uncalibrated reconstruction method, in which no calibration information is used
 *       for this case, we can use either canonical reconstruction, or a reference plane reconstruction
 * after the camera is reconstructed, we can do triangulation to get 3D points. user need to input a threhold 
 *   of reprojection error to discern inliers and outliers
 *
 */
class TwoViewReconstructorImpl : public TwoViewReconstructor {
public:
	//responsible for deleting the parameter objects
	TwoViewReconstructorImpl(
		FundamentalMatrixCalculator* fmc,
		CameraReconstructor* cameraReconstructor,
		PointReconstructor* pointReconstructor
		);
	~TwoViewReconstructorImpl(void);
public:
	bool compute(const PointPairs& pps);

	const TwoViewReconstructionResults& getReconstructionResults() const { return results; }

public: //set parameters

private:
	TwoViewReconstructionResults results;

private:
	FundamentalMatrixCalculator* fmc;
	PointReconstructor* pointReconstructor;
	CameraReconstructor* cameraReconstructor;
};

/**
Two flags:
1. inputNormalized. If input is already normalized, then the returned cameras has internal matrix I, else as specified
2. assumeTranslation. If assume translation, then E = [t]x, R = I. The computation is more simple and more robust.
*/
class CalibratedTwoViewReconstructorImpl : public TwoViewReconstructor {
public:
	//responsible for deleting the parameter objects
	CalibratedTwoViewReconstructorImpl(
		const CvMat* K1, 
		const CvMat* K2, 
		double thresholdOfEssentialMatrixCalculation,
		double thresholdOfReprojectionError);

	~CalibratedTwoViewReconstructorImpl(void);

public:
	//if the input is already normalized, then do not do normalization based on K1 and K2 again
	//default is false;
	void setInputNormalized(bool v) { this->inputIsNormalized = v; }

	//if assume translation, then E = [t]x, P1 = K1[I|0], and P2 = K2[I|t]
	void setAssumeTranslation(bool v) { this->assumeTranslation = v; }

	bool compute(const PointPairs& pps);

	const TwoViewReconstructionResults& getReconstructionResults() const { return results; }

public: //set parameters

private:
	void computeCameras(const std::vector<CvPoint2D32f>& nx1s, const std::vector<CvPoint2D32f>& nx2s);
	void computePoints(const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s);
	//P = K[R|t]
	Camera getCameraFromEssentialMatrix(const CvMat* E,  const std::vector<CvPoint2D32f>& nx1s, const std::vector<CvPoint2D32f>& nx2s, const CvMat* K);

private:
	TwoViewReconstructionResults results;

private:
	CvMatWrapper K1;
	CvMatWrapper K2;
	bool inputIsNormalized; 
	bool assumeTranslation;
	double thresholdOfEssentialMatrixCalculation;
	double thresholdOfReprojectionError;
};

}