#pragma once

#include "PointReconstructor.h"

#include <fvision/geom.h>

#include <vector>

namespace fvision {

class PointReconstructionResults;

class TwoViewReconstructionResults {
public:
	std::vector<CvPoint2D32f> getFInliersOnLeftImage() const;
	std::vector<CvPoint2D32f> getFInliersOnRightImage() const;
	std::vector<CvPoint2D32f> getFOutliersOnLeftImage() const;
	std::vector<CvPoint2D32f> getFOutliersOnRightImage() const;
	PointPairs getFInliers() const;
	PointPairs getFOutliers() const;

	//x' = K * x
	//P = K * NP
	//F = K2' * E * K1
	TwoViewReconstructionResults* getDenormalizedResults(const CvMat* K1, const CvMat* K2) const ;

public:
	CvMatWrapper fw; //fundamental matrix
	std::vector<Camera> cameras; //only two cameras
	PointReconstructionResults pointResults; 

	//inlier and outlier during F computation
	std::vector<int> fInlierIndices;
	std::vector<int> fOutlierIndices;
};

/**
 * Given image point correspondences, assumes no lens distortion
 * do 3D reconstruction
 */
class TwoViewReconstructor {
public:
	virtual ~TwoViewReconstructor() {}
	//return false if failed
	virtual bool compute(const PointPairs& pps) = 0;
	virtual const TwoViewReconstructionResults& getReconstructionResults() const = 0;
};


}