#include "HomographyCalculatorFactory.h"

#include "HomographyCalculatorImpl.h"

#include <iostream>

using namespace std;

namespace fvision {

HomographyCalculator* HomographyCalculatorFactory::create(const Configuration& configuration) {

	assert (configuration.isValid());

	HomographyCalculator* calculator = NULL;

	switch (configuration.method) {
	case Configuration::DLT: {
		calculator = new DltHomographyCalculator();
		break; }
	case Configuration::RANSAC: {
		calculator = RANSAC_HomographyCalculator::create(configuration.ransacDistanceThreshold, configuration.ransacModelSampleSize); 
		break;}
	default: {
		calculator = RANSAC_HomographyCalculator::create(configuration.ransacDistanceThreshold, configuration.ransacModelSampleSize); 
		break; }
	}

	if (configuration.normalize) {
		return new NormalizedHomographyCalculator(calculator);
	} else {
		return calculator;
	}
}

HomographyCalculator* HomographyCalculatorFactory::createDLT() { return new DltHomographyCalculator(); }

HomographyCalculator* HomographyCalculatorFactory::createRansac(double threshold) {
	return new NormalizedHomographyCalculator(RANSAC_HomographyCalculator::create(threshold));
}

std::ostream& operator<<(std::ostream& os, const HomographyCalculatorFactory::Configuration& config) {
	os<<"method: ";
	switch(config.method) {
		case HomographyCalculatorFactory::Configuration::DLT: cout<<"DLT"<<endl; break;
		case HomographyCalculatorFactory::Configuration::RANSAC: cout<<"RANSAC"<<endl; break;
		default: cout<<"DEFAUTE"<<endl; break;
	}
	cout<<"normalized: "<<config.normalize<<endl;
	cout<<"ransac distance threshold: "<<config.ransacDistanceThreshold<<endl;
	return os;
}

}