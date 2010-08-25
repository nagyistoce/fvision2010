#include "FundamentalMatrixCalculatorFactory.h"

#include "FundamentalMatrixCalculatorImpl.h"

#include <iostream>

using namespace std;

namespace fvision {

FundamentalMatrixCalculator* FundamentalMatrixCalculatorFactory::create(const Configuration& configuration) {

	assert (configuration.isValid());

	FundamentalMatrixCalculator* fmc = NULL;

	switch (configuration.method) {
	case Configuration::DLT: {
		if (configuration.essential) {
			fmc = new DltEssentialMatrixCalculator();
		} else {
			fmc = new DltFundamentalMatrixCalculator();
		}
		break; }
	case Configuration::RANSAC: {
		if (configuration.essential) {
			fmc = RANSAC_FundamentalMatrixCalculator::createEssential(configuration.ransacDistanceThreshold, configuration.ransacModelSampleSize); 
		} else {
			fmc = RANSAC_FundamentalMatrixCalculator::create(configuration.ransacDistanceThreshold, configuration.ransacModelSampleSize); 
		}		
		break;}
	case Configuration::HOMOGRAPHY_BASED: {
		fmc = new HomographyBasedFundamentalMatrixCalculator(configuration.ransacDistanceThreshold);
		if (configuration.essential) ((HomographyBasedFundamentalMatrixCalculator*)fmc)->setEssential(true);
		break;
										  }
	case Configuration::ASSUME_TRANSLATION: {
		fmc = new IntersectionBasedFundamentalMatrixCalculator(configuration.ransacDistanceThreshold);
		break;
											}
	default: {
		fmc = RANSAC_FundamentalMatrixCalculator::create(configuration.ransacDistanceThreshold, configuration.ransacModelSampleSize); 
		break; }
	}

	if (configuration.normalize && !configuration.essential && configuration.method != Configuration::ASSUME_TRANSLATION) {
		return new NormalizedFundamentalMatrixCalculator(fmc);
	} else {
		return fmc;
	}
}

std::ostream& operator<<(std::ostream& os, const FundamentalMatrixCalculatorFactory::Configuration& config) {
	os<<"method: ";
	switch(config.method) {
		case FundamentalMatrixCalculatorFactory::Configuration::DLT: cout<<"DLT"<<endl; break;
		case FundamentalMatrixCalculatorFactory::Configuration::RANSAC: cout<<"RANSAC"<<endl; break;
		case FundamentalMatrixCalculatorFactory::Configuration::HOMOGRAPHY_BASED: cout<<"HOMOGRAPHY_BASED"<<endl; break;
		case FundamentalMatrixCalculatorFactory::Configuration::ASSUME_TRANSLATION: cout<<"ASSUME_TRANSLATION"<<endl; break;
		default: cout<<"DEFAUTE"<<endl; break;
	}
	cout<<"normalized: "<<config.normalize<<endl;
	cout<<"essential: "<<config.essential<<endl;
	cout<<"ransac distance threshold: "<<config.ransacDistanceThreshold<<endl;
	return os;
}

}