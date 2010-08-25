#include "LineSegmentIntersectionCalculatorFactory.h"

#include "LineSegmentIntersectionCalculatorImpl.h"

namespace fvision {

LineSegmentIntersectionCalculator* LineSegmentIntersectionCalculatorFactory::create(const Configuration& configuration) {

	assert (configuration.isValid());

	LineSegmentIntersectionCalculator* calculator = NULL;

	switch (configuration.method) {
		case Configuration::DLT: {
			calculator = new DltLineSegmentIntersectionCalculator();
			break; }
		case Configuration::RANSAC: {
			calculator = RANSAC_LineSegmentIntersectionCalculator::create(configuration.ransacDistanceThreshold); 
			break;}
		default: {
			calculator = RANSAC_LineSegmentIntersectionCalculator::create(configuration.ransacDistanceThreshold); 
			break; }
	}

	if (configuration.normalize) {
		return new NormalizedLineSegmentIntersectionCalculator(calculator);
	} else {
		return calculator;
	}
}
}
