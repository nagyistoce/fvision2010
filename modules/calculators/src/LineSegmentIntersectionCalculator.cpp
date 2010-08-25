#include "LineSegmentIntersectionCalculator.h"

namespace fvision {

LineSegmentIntersectionGeometricErrorCalculator::LineSegmentIntersectionGeometricErrorCalculator(int metric /*= 1*/) {
	assert (metric == 1 || metric == 2);
	this->metric = metric;
	midPointBuf = cvCreateMat(3, 1, CV_64FC1);
	cvmSet(midPointBuf, 2, 0, 1.0);
	lineBuf = cvCreateMat(3, 1, CV_64FC1);
}

LineSegmentIntersectionGeometricErrorCalculator::~LineSegmentIntersectionGeometricErrorCalculator() {
	cvReleaseMat(&midPointBuf);
	cvReleaseMat(&lineBuf);
}

double LineSegmentIntersectionGeometricErrorCalculator::compute(const CvMatWrapper& fw, const PointPair& sample) const {
	cvmSet(midPointBuf, 0, 0, (sample.first.x + sample.second.x) / 2);
	cvmSet(midPointBuf, 1, 0, (sample.first.y + sample.second.y) / 2);
	cvCrossProduct(fw.getMat(), midPointBuf, lineBuf);
	return point2LineDistance(sample.first, lineBuf, metric);
}

double LineSegmentIntersectionAlgebraicErrorCalculator::compute(const CvMatWrapper& fw, const PointPair& sample) const {
	getLine(sample.first, sample.second, lineBuf);
	return abs(cvDotProduct(fw.getMat(), lineBuf));
}

LineSegmentIntersectionAlgebraicErrorCalculator::LineSegmentIntersectionAlgebraicErrorCalculator() {
	lineBuf = cvCreateMat(3, 1, CV_64FC1);
}

LineSegmentIntersectionAlgebraicErrorCalculator::~LineSegmentIntersectionAlgebraicErrorCalculator() {
	cvReleaseMat(&lineBuf);
}
}