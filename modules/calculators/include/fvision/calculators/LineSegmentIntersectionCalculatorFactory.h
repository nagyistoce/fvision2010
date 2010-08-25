#ifndef FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_FACTORY_H_
#define FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_FACTORY_H_

#include "LineSegmentIntersectionCalculator.h"

namespace fvision {

class LineSegmentIntersectionCalculatorFactory {
public:
	class Configuration {		
	public:
		Configuration() {
			method = RANSAC;
			normalize = true;
			ransacDistanceThreshold = 0.0015;
		}

		bool isValid() const {
			return ransacDistanceThreshold >0;
		}

	public:
		enum Method {
			DLT,
			RANSAC
		};

	public:
		Method method;
		bool normalize;
		double ransacDistanceThreshold;
	};
public:
	LineSegmentIntersectionCalculator* create(const Configuration& configuration);
};

}
#endif // FVISION_LINE_SEGMENT_INTERSECTION_CALCULATOR_FACTORY_H_
