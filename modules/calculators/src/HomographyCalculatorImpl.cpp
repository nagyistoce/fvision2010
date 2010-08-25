#include "HomographyCalculatorImpl.h"

namespace fvision {


//<<multiple view geometry>>(2nd) p89
class HomographyConstraints : public ConstraintsConstructor<PointPair> {
private:
	CvMat* A;

public:
	HomographyConstraints() {
		A = cvCreateMat(2, 9, CV_64FC1);
	}
	~HomographyConstraints() {
		cvReleaseMat(&A);
	}

public:
	const CvMat* construct(const PointPair& sample) const {
		double x1 = sample.first.x;
		double y1 = sample.first.y;
		double x2 = sample.second.x;
		double y2 = sample.second.y;

		cvZero(A);

		cvmSet(A, 0, 3, -x1);
		cvmSet(A, 0, 4, -y1);
		cvmSet(A, 0, 5, -1);
		cvmSet(A, 0, 6, y2 * x1);
		cvmSet(A, 0, 7, y2 * y1);
		cvmSet(A, 0, 8, y2);

		cvmSet(A, 1, 0, x1);
		cvmSet(A, 1, 1, y1);
		cvmSet(A, 1, 2, 1);
		cvmSet(A, 1, 6, -x2 * x1);
		cvmSet(A, 1, 7, -x2 * y1);
		cvmSet(A, 1, 8, -x2);

		return A;
	}
};

CvMatWrapper DltHomographyCalculator::compute(const PointPairs& samples) {

	HomographyConstraints c;
	GenericDlt<PointPair> dlt;
	CvMat* h = dlt.compute(samples, c);

	CvMat row_header, *H;
	H = cvReshape(h, &row_header, 0, 3);

	CvMatWrapper fw(H);

	cvReleaseMat(&h);

	return fw;
}

//H = inv(T2) * nH * T1
CvMat* HomographyDenormalizer::denormalize(const CvMat* nmodel, const CvMat* T1, const CvMat* T2) {
	CvMat* invT2 = inv(T2);
	CvMat* H = matMul(invT2, nmodel, T1);
	cvReleaseMat(&invT2);
	return H;
}

}