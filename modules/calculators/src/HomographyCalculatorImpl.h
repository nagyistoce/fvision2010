#ifndef FVISION_HOMOGRAPHY_CALCULATOR_IMPL_H_
#define FVISION_HOMOGRAPHY_CALCULATOR_IMPL_H_

#include "HomographyCalculator.h"

namespace fvision {

class DltHomographyCalculator : public HomographyCalculator {
public:
	CvMatWrapper compute(const PointPairs& samples);
};

class RANSAC_HomographyCalculator : public Ransac<CvMatWrapper, PointPair> {
public:
	RANSAC_HomographyCalculator(HomographyCalculator* pmc, int modelSampleSize, HomographyErrorCalculator* pec) 
        : Ransac<CvMatWrapper, PointPair>(pmc, modelSampleSize, pec) {
	}

	static RANSAC_HomographyCalculator* create(double threshold, int modelSampleSize = 5, int metric = 1) {
		HomographyGeometricErrorCalculator* pec = new HomographyGeometricErrorCalculator(metric);
		pec->setThreshold(threshold);
		return new RANSAC_HomographyCalculator(new DltHomographyCalculator(), modelSampleSize, pec);
	}

public:
	~RANSAC_HomographyCalculator(void) {}
};

/**
* @see TwoViewModelDenormalizer
*/
class HomographyDenormalizer : public TwoViewModelDenormalizer {
public:
	CvMat* denormalize(const CvMat* nmodel, const CvMat* T1, const CvMat* T2);
};

class NormalizedHomographyCalculator : public NormalizedTwoViewModelCaculator {
public:
	NormalizedHomographyCalculator(HomographyCalculator* pmc) : NormalizedTwoViewModelCaculator(pmc, new HomographyDenormalizer()) {
	}
};


}
#endif // FVISION_HOMOGRAPHY_CALCULATOR_IMPL_H_
