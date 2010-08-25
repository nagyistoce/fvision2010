#pragma once

#include "TwoViewModelCalculator.h"

/**
compute fundamental matrix

@see FundamentalMatrixCalculatorFactory
*/
namespace fvision {

typedef TwoViewModelCalculator FundamentalMatrixCalculator;

typedef TwoViewModelErrorCalculator FundamentalMatrixErrorCalculator;

class FundamentalMatrixGeometricErrorCalculator : public FundamentalMatrixErrorCalculator {
public:
	FundamentalMatrixGeometricErrorCalculator(int metric = 1) { this->metric = metric; }
public:
	double compute(const CvMatWrapper& fw, const PointPair& sample) const;
private:
	int metric;
};

class FundamentalMatrixAlgebraicErrorCalculator : public FundamentalMatrixErrorCalculator {
public:
	double compute(const CvMatWrapper& fw, const PointPair& sample) const;
};

typedef ModelEvaluationResults<PointPair> FundamentalMatrixEvaluationResults;
typedef ModelEvaluator<CvMatWrapper, PointPair, FundamentalMatrixGeometricErrorCalculator> FundamentalMatrixGeometricErrorEvaluator;
typedef ModelEvaluator<CvMatWrapper, PointPair, FundamentalMatrixAlgebraicErrorCalculator> FundamentalMatrixAlgebraicErrorEvaluator;

}