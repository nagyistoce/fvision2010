#include "FundamentalMatrixCalculator.h"

namespace fvision {

double FundamentalMatrixGeometricErrorCalculator::compute(const CvMatWrapper& fw, const PointPair& sample) const {

	CvMat* mx1 = hmatFromPoint2D(sample.first);
	CvMat* mx2 = hmatFromPoint2D(sample.second);

	CvMat* Fx1 = matMul(fw.getMat(), mx1);

	CvMat* Ft = transpose(fw.getMat());

	CvMat* Ftx2 = matMul(Ft, mx2);

	double d1 = point2LineDistance(mx2, Fx1); 
	double d2 = point2LineDistance(mx1, Ftx2);
	double d = (d1 + d2) / 2;
	if (metric == 2) {
		d = (d1 * d1 + d2 * d2) / 2;
	}

	cvReleaseMat(&mx1);
	cvReleaseMat(&mx2);
	cvReleaseMat(&Fx1);
	cvReleaseMat(&Ft);
	cvReleaseMat(&Ftx2);

	return d;
}

double FundamentalMatrixAlgebraicErrorCalculator::compute(const CvMatWrapper& fw, const PointPair& sample) const {

	const CvMat* F = fw.getMat();

	double mx1[] = {sample.first.x, sample.first.y, 1.0};
	double mx2[] = {sample.second.x, sample.second.y, 1.0};

	double sum = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sum += cvmGet(F, i, j) * mx2[i] * mx1[j];
		}
	}
	return abs(sum);
}

}
