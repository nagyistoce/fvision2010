#include "TwoViewModelCalculator.h"

using namespace std;

namespace fvision {

NormalizedTwoViewModelCaculator::NormalizedTwoViewModelCaculator(TwoViewModelCalculator* pmc, TwoViewModelDenormalizer* pmd, bool equalT) {
	this->pmc = pmc;
	this->pmd = pmd;
	this->equalT = equalT;
	T1 = cvCreateMat(3, 3, CV_64FC1);
	T2 = cvCreateMat(3, 3, CV_64FC1);
}

NormalizedTwoViewModelCaculator::~NormalizedTwoViewModelCaculator() {
	delete pmc;
	delete pmd;
	cvReleaseMat(&T1);
	cvReleaseMat(&T2);
}

CvMatWrapper NormalizedTwoViewModelCaculator::compute(const PointPairs& pps) {

	vector<CvPoint2D32f> x1s, x2s;
	decomposePointPairs(pps, x1s, x2s);
	vector<CvPoint2D32f> nx1s, nx2s;
	normalize(x1s, nx1s, T1);

	if (equalT) {
		cvCopy(T1, T2);
		nx2s = transformPoints(T2, x2s);
	} else {
		normalize(x2s, nx2s, T2);
	}

	PointPairs npps;
	composePointPairs(npps, nx1s, nx2s);

	double originalThreshold = 0;
	if (pmc->hasThreshold()) {
		originalThreshold = pmc->getThreshold();
		double scale = (getScale1() + getScale2()) / 2;
		pmc->setThreshold(originalThreshold * scale);
	}

	CvMatWrapper nmodel = pmc->compute(npps);

	if (pmc->hasThreshold()) {
		pmc->setThreshold(originalThreshold);
	}

	CvMat* model = pmd->denormalize(nmodel.getMat(), T1, T2);

	CvMatWrapper mw(model);

	cvReleaseMat(&model);
	return mw;
}

double NormalizedTwoViewModelCaculator::getScale1() const {
	return cvmGet(T1, 0, 0);	
}

double NormalizedTwoViewModelCaculator::getScale2() const {
	return cvmGet(T1, 0, 0);
}

}