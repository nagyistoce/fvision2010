#ifndef FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_IMPL_H_
#define FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_IMPL_H_

#include "LineSegmentIntersectionCalculator.h"

namespace fvision {

/**
if the length of the segment is less than a threshold, it is a bad sample and we need to discard it
*/
class DltLineSegmentIntersectionCalculator : public LineSegmentIntersectionCalculator {
public:
	CvMatWrapper compute(const PointPairs& samples);
};

class RANSAC_LineSegmentIntersectionCalculator : public Ransac<CvMatWrapper, PointPair> {
public:
	RANSAC_LineSegmentIntersectionCalculator(LineSegmentIntersectionCalculator* pmc, int modelSampleSize, LineSegmentIntersectionErrorCalculator* pec) 
        : Ransac<CvMatWrapper, PointPair>(pmc, modelSampleSize, pec) {
	}

	static RANSAC_LineSegmentIntersectionCalculator* create(double threshold, int modelSampleSize = 2, int metric = 1) {
		LineSegmentIntersectionErrorCalculator* pec = new LineSegmentIntersectionGeometricErrorCalculator(metric);
		pec->setThreshold(threshold);
		return new RANSAC_LineSegmentIntersectionCalculator(new DltLineSegmentIntersectionCalculator(), modelSampleSize, pec);
	}

public:
	~RANSAC_LineSegmentIntersectionCalculator(void) {}
};

class LineSegmentIntersectionDenormalizer : public TwoViewModelDenormalizer {
private:
	CvMat* invT;
public:
	LineSegmentIntersectionDenormalizer() { invT = cvCreateMat(3, 3, CV_64FC1); }
	~LineSegmentIntersectionDenormalizer() { cvReleaseMat(&invT); }

public:
	CvMat* denormalize(const CvMat* nmodel, const CvMat* T1, const CvMat* T2);
};


class NormalizedLineSegmentIntersectionCalculator : public NormalizedTwoViewModelCaculator {
public:
	NormalizedLineSegmentIntersectionCalculator(LineSegmentIntersectionCalculator* pfmc) : NormalizedTwoViewModelCaculator(pfmc, new LineSegmentIntersectionDenormalizer(), true) {
	}
};


}
#endif // FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_IMPL_H_
