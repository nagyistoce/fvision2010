#pragma once

#include "FundamentalMatrixCalculator.h"

#include <cv.h>

#include <vector>
#include <iostream>


namespace fvision {

class DltFundamentalMatrixCalculator : public FundamentalMatrixCalculator {
public:
	CvMatWrapper compute(const PointPairs& samples);
};

//assume the samples are normalized with K
class DltEssentialMatrixCalculator : public FundamentalMatrixCalculator {
public:
	CvMatWrapper compute(const PointPairs& samples);
};

class RANSAC_FundamentalMatrixCalculator : public Ransac<CvMatWrapper, PointPair> {
public:
	RANSAC_FundamentalMatrixCalculator(FundamentalMatrixCalculator* pmc, int modelSampleSize, FundamentalMatrixErrorCalculator* pec) 
        : Ransac<CvMatWrapper, PointPair>(pmc, modelSampleSize, pec) {
	}

	static RANSAC_FundamentalMatrixCalculator* create(double threshold, int modelSampleSize = 8) {
		FundamentalMatrixGeometricErrorCalculator* pec = new FundamentalMatrixGeometricErrorCalculator();
		pec->setThreshold(threshold);
		return new RANSAC_FundamentalMatrixCalculator(new DltFundamentalMatrixCalculator(), modelSampleSize, pec);
	}

	static RANSAC_FundamentalMatrixCalculator* createEssential(double threshold, int modelSampleSize = 8) {
		FundamentalMatrixGeometricErrorCalculator* pec = new FundamentalMatrixGeometricErrorCalculator();
		pec->setThreshold(threshold);
		return new RANSAC_FundamentalMatrixCalculator(new DltEssentialMatrixCalculator(), modelSampleSize, pec);
	}

public:
	~RANSAC_FundamentalMatrixCalculator(void) {}
};

class FundamentalMatrixDenormalizer : public TwoViewModelDenormalizer {
public:
	CvMat* denormalize(const CvMat* nmodel, const CvMat* T1, const CvMat* T2);
};


class NormalizedFundamentalMatrixCalculator : public NormalizedTwoViewModelCaculator {
public:
	NormalizedFundamentalMatrixCalculator(TwoViewModelCalculator* pfmc) : NormalizedTwoViewModelCaculator(pfmc, new FundamentalMatrixDenormalizer()) {
	}
};

class IntersectionBasedFundamentalMatrixCalculator : public FundamentalMatrixCalculator {
public:
	IntersectionBasedFundamentalMatrixCalculator(double ransacThreshold);

	virtual bool hasThreshold() const { return true; }

	virtual void setThreshold(double threshold) {
		ransacThreshold = threshold;
	}

	virtual double getThreshold() const {
		return ransacThreshold;
	}

public:
	CvMatWrapper compute(const PointPairs& samples);

	const std::vector<int>& getInlierIndices() const { return inlierIndices; }
	const std::vector<int>& getOutlierIndices() const { return outlierIndices; }

private:
	double ransacThreshold;
	CvMatWrapper e;
	std::vector<int> inlierIndices;
	std::vector<int> outlierIndices;
};

//first F using ransac; //fInliers, fOutliers, fInlierIndices, fOutlierIndices
//then compute a dominant homography from the inliers;// hInliers, hOutliers, hInlierIndices, hOutlierIndices
//lastly, use outliers to compute an epipole, //hOutlierIndices + fOutlierIndices -> eInlierIndices, eOutlierIndices, eInliers, eOutliers
class HomographyBasedFundamentalMatrixCalculator : public FundamentalMatrixCalculator {
public:
	HomographyBasedFundamentalMatrixCalculator(double ransacThreshold);

public:
	virtual bool hasThreshold() const { return true; }
	virtual void setThreshold(double threshold);
	virtual double getThreshold() const { return ransacThreshold; }

	//if essential is set, then a essential matrix is returned by normalizing the fundamental matrix
	//                       and assuming the input image point coordinates are normalized
	void setEssential(bool v) {
		this->essentialMatrix = v;
	}

	bool isEssential() const { return this->essentialMatrix; }

	CvMatWrapper compute(const PointPairs& samples);

	const std::vector<int>& getInlierIndices() const { return finalFInlierIndices; }
	const std::vector<int>& getOutlierIndices() const { return finalFOutlierIndices; }

public:
	std::vector<int> initialFInlierIndices;
	std::vector<int> initialFOutlierIndices;
	PointPairs initialFInliers;
	PointPairs initialFOutliers;
	double initialFInliersRate;

	std::vector<int> hInlierIndices;
	std::vector<int> hOutlierIndices;
	PointPairs hInliers;
	PointPairs hOutliers;
	double hInliersRateInF;
	CvMatWrapper H;

	std::vector<int> eInlierIndices;
	std::vector<int> eOutlierIndices;
	PointPairs eInliers;
	PointPairs eOutliers;
	double eInliersRate;
	CvMatWrapper e;

	std::vector<int> finalFInlierIndices;
	std::vector<int> finalFOutlierIndices;

private:
	double ransacThreshold;

private:
	double fRansacThreshold;
	double hRansacThreshold;
	double eRansacThreshold;

	bool essentialMatrix; //if true, return essential matrix by normalize the fundamental matrix
};

}
