#ifndef FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_H_
#define FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_H_

#include "TwoViewModelCalculator.h"

/**
Given a set of line segments, compute the intersection point
a line segment is represented by a point pair <x1, x2>
for a point cx lies on the line segment, we have the following constraint:

cx .* (x1 x x2) = 0

where .* means dot product and x means cross product

@see LineSegmentIntersectionCalculatorFactory
*/
namespace fvision {

typedef TwoViewModelCalculator LineSegmentIntersectionCalculator;

typedef TwoViewModelErrorCalculator LineSegmentIntersectionErrorCalculator;

class LineSegmentIntersectionGeometricErrorCalculator : public LineSegmentIntersectionErrorCalculator {
public:
	LineSegmentIntersectionGeometricErrorCalculator(int metric = 1);
	~LineSegmentIntersectionGeometricErrorCalculator();
public:
	double compute(const CvMatWrapper& fw, const PointPair& sample) const;
private:
	int metric;
	CvMat* midPointBuf;
	CvMat* lineBuf;
};

class LineSegmentIntersectionAlgebraicErrorCalculator : public LineSegmentIntersectionErrorCalculator {
public:
	LineSegmentIntersectionAlgebraicErrorCalculator();
	~LineSegmentIntersectionAlgebraicErrorCalculator();
public:
	double compute(const CvMatWrapper& fw, const PointPair& sample) const;
private:
	CvMat* lineBuf;
};

typedef ModelEvaluationResults<PointPair> LineSegmentIntersectionEvaluationResults;
typedef ModelEvaluator<CvMatWrapper, PointPair, LineSegmentIntersectionGeometricErrorCalculator> LineSegmentIntersectionGeometricErrorEvaluator;
typedef ModelEvaluator<CvMatWrapper, PointPair, LineSegmentIntersectionAlgebraicErrorCalculator> LineSegmentIntersectionAlgebraicErrorEvaluator;

}

#endif // FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_H_