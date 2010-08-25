#ifndef FVISION_HOMOGRAPHY_CALCULATOR_H_
#define FVISION_HOMOGRAPHY_CALCULATOR_H_

#include "TwoViewModelCalculator.h"

namespace fvision {

typedef TwoViewModelCalculator HomographyCalculator;

typedef TwoViewModelErrorCalculator HomographyErrorCalculator;

class HomographyGeometricErrorCalculator : public HomographyErrorCalculator {
public:
	/// metric can only be 1 or 2, 1 for absolute distance and 2 for square distance
	HomographyGeometricErrorCalculator(int metric = 1);

public:
	double compute(const CvMatWrapper& fw, const PointPair& sample) const;

private:
	int metric;
};

typedef ModelEvaluationResults<PointPair> HomographyEvaluationResults;
typedef ModelEvaluator<CvMatWrapper, PointPair, HomographyGeometricErrorCalculator> HomographyGeometricErrorEvaluator;

}

#endif // FVISION_HOMOGRAPHY_CALCULATOR_H_
