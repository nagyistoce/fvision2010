#pragma once

#include <fvision/moca.h>
#include <fvision/cvutils.h>
#include <fvision/geom.h>

namespace fvision {

typedef GenericModelCalculator<CvMatWrapper, PointPair> TwoViewModelCalculator;
typedef GenericErrorCaclculator<CvMatWrapper, PointPair> TwoViewModelErrorCalculator;

/** Interface for a normalized two view model calculator, such as fundamental matrix, homography
* 
* @see normalize, NormalizedTwoViewModelCaculator
*/
class TwoViewModelDenormalizer {
public:
	virtual CvMat* denormalize(const CvMat* nmodel, const CvMat* T1, const CvMat* T2) = 0;
};

/** A generic two view model caluclator with first normalizing points
* first normalize the image points, nx1 = T1 * x1; nx2 = T2 * x2, such that nx1s and nx2s are centered at origin and has mean radius is sqrt(2)
* then compute a model using the normalized image points, say nmodel <- {nx1s, nx2s}
* transform nmodel back with T1 and T2, such that model <- {x1s, x2s}
*    for fundamental matrix: suppose {nx1s, nx2s} -> nF, i.e. nx2' * nF * nx1 = 0 then F = T2' * nF * T1  (see <<multiple view geometry>> 2nd. p282)
*                            nx2' * nF * nx1 = x2' * T2' * nF * T1 * x1 = x2' * F * x1 = 0
*    for homography, suppose {nx1s, nx2s} -> nH, i.e. nx2 = nH * nx1, then H = inv(T2) * nH * T1. (see <<multiple view geometry>> 2nd. p109)
*                            T2 * x2 = nH * T1 * x1 --> x2 = inv(T2) * nH * T1 * x1 = H * x1
*
* @see ModelDenormalizer, NormalizedFundamentalMatrixCalculator, NormalizedHomographyCalculator
*/
class NormalizedTwoViewModelCaculator : public TwoViewModelCalculator {
public:
	/**
	* when equalT == true, we only compute T1 for first point set and then apply T1 to second point set
	* responsible for deleting pmc and pmd
	*/
	NormalizedTwoViewModelCaculator(TwoViewModelCalculator* pmc, TwoViewModelDenormalizer* pmd, bool equalT = false);

	~NormalizedTwoViewModelCaculator();

public:
	CvMatWrapper compute(const PointPairs& pps);

	//normalize x1s with T, use T for x2s
	void setEqualT(bool v) {
		equalT = v;
	}

	double getScale1() const;

	double getScale2() const;

	const std::vector<int>& getInlierIndices() const { return pmc->getInlierIndices(); };
	const std::vector<int>& getOutlierIndices() const { return pmc->getOutlierIndices(); };

	TwoViewModelCalculator* getModelCalculator() const { return pmc; }

private:
	TwoViewModelCalculator* pmc;
	TwoViewModelDenormalizer* pmd;
	bool equalT;
	CvMat* T1;
	CvMat* T2;
};

}