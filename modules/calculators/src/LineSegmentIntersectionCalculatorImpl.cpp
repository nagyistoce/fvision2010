#include "LineSegmentIntersectionCalculatorImpl.h"

namespace fvision {

class LineSegmentIntersectionConstraints : public ConstraintsConstructor<PointPair> {
private:
	CvMat* A;

public:
	LineSegmentIntersectionConstraints() {
		A = cvCreateMat(1, 3, CV_64FC1);
	}
	~LineSegmentIntersectionConstraints() {
		cvReleaseMat(&A);
	}
public:
	const CvMat* construct(const PointPair& sample) const {
		cvmSet(A, 0, 0, sample.first.y - sample.second.y);
		cvmSet(A, 0, 1, -sample.first.x + sample.second.x);
		cvmSet(A, 0, 2, sample.first.x * sample.second.y - sample.first.y * sample.second.x);
		return A;
	}
};

CvMatWrapper DltLineSegmentIntersectionCalculator::compute(const PointPairs& samples) {
	LineSegmentIntersectionConstraints c;
	GenericDlt<PointPair> dlt;
	CvMat* h = dlt.compute(samples, c);
	CvMatWrapper hw(h);
	cvReleaseMat(&h);
	return hw;
}


CvMat* LineSegmentIntersectionDenormalizer::denormalize(const CvMat* nmodel, const CvMat* T1, const CvMat* T2) {
	cvInv(T1, invT);
	CvMat* model = matMul(invT, nmodel);
	return model;
}
}
