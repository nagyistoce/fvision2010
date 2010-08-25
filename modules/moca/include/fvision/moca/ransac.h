#ifndef FVISION_RANSAC_H_
#define FVISION_RANSAC_H_

#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

namespace fvision {

class RANSAC_SamplesNumber {
public:
	RANSAC_SamplesNumber(int modelSampleSize) {
		this->s = modelSampleSize;
		this->p = 0.99;
	}
	~RANSAC_SamplesNumber(void) {}

public:
	long calcN(int inliersNumber, int samplesNumber) {
		double e = 1 - (double)inliersNumber / samplesNumber;
		//cout<<"e: "<<e<<endl;
		if (e > 0.9) e = 0.9;
		//cout<<"pow: "<<pow((1 - e), s)<<endl;
		//cout<<log(1 - pow((1 - e), s))<<endl;
		long N = (long)(log(1 - p) / log(1 - pow((1 - e), s)));
		if (N < 0) return (long)1000000000;
		else return N;
	}

private:		
	int s;      //samples size for fitting a model
	double p;   //probability that at least one of the random samples if free from outliers
	            //usually 0.99
};

/** generic RANSAC framework
 * make use of a model calculator and an error calculator
 * M is the model type, need to support copy assignment operator and default constructor
 * S is the sample type.
 *
 * Interface:
 *  M compute(samples); input a set of samples, output a model. 
 *  after compute, inliers and outliers can be retrieved
 * 
 * Example:
 * 
 */
template <typename M, typename S>
class Ransac : public GenericModelCalculator<M, S> {
public:
	typedef std::vector<S> Samples;

	/** Constructor
	 * 
	 * @param pmc a GenericModelCalculator object
	 * @param modelSampleSize how much samples are used to fit a model
	 * @param pec a GenericErrorCaclculator object
	 */
	Ransac(GenericModelCalculator<M, S>* pmc, int modelSampleSize, GenericErrorCaclculator<M, S>* pec) {
		this->pmc = pmc;
		this->modelSampleSize = modelSampleSize;
		this->pec = pec;
		this->maxSampleCount = 500;
		this->minInliersNum = 1000000;

		this->verbose = false;
	}

	const GenericErrorCaclculator<M, S>* getErrorCalculator() const { return pec; }

	virtual ~Ransac() {
		delete pmc;
		delete pec;
	}

	void setMaxSampleCount(int n) {
		this->maxSampleCount = n;
	}

	void setMinInliersNum(int n) {
		this->minInliersNum = n;
	}

	virtual bool hasThreshold() const { return true; }

	virtual void setThreshold(double threshold) {
		pec->setThreshold(threshold);
	}

	virtual double getThreshold() const {
		return pec->getThreshold();
	}

public:
	/// Given samples, compute a model that has most inliers. Assume the samples size is larger or equal than model sample size
	/// inliers, outliers, inlierIndices and outlierIndices are stored
	M compute(const Samples& samples) {
		clear();

		int pointsNumber = (int)samples.size();

		assert(pointsNumber >= modelSampleSize);

		long N = 100000;
		int sampleCount = 0;
		RANSAC_SamplesNumber ransac(modelSampleSize);

		M bestModel;
		int maxInliersNumber = 0;

		bool stop = false;
		while (sampleCount < N && sampleCount < maxSampleCount && !stop) {

			Samples nsamples;
			randomlySampleN(samples, nsamples, modelSampleSize);

			M sampleModel = pmc->compute(nsamples);
			if (maxInliersNumber == 0) bestModel = sampleModel;  //init bestModel

			int inliersNumber = pec->calcInliersNumber(sampleModel, samples);
			if (verbose) std::cout<<"inliers number: "<<inliersNumber<<std::endl;

			if (inliersNumber > maxInliersNumber) {
				bestModel = sampleModel;
				maxInliersNumber = inliersNumber;
				N = ransac.calcN(inliersNumber, pointsNumber);
				if (maxInliersNumber > minInliersNum) stop = true;
			}

			if (verbose) std::cout<<"N: "<<N<<std::endl;

			sampleCount ++;
		}

		if (verbose) std::cout<<"sampleCount: "<<sampleCount<<std::endl;

		finalModel = computeUntilConverge(bestModel, maxInliersNumber, samples);
		
		pec->classify(finalModel, samples, inlierIndices, outlierIndices, inliers, outliers);

		inliersRate = (double)inliers.size() / samples.size();

		return finalModel;
	}

	const Samples& getInliers() const { return inliers; }
	const Samples& getOutliers() const { return outliers; }

	const std::vector<int>& getInlierIndices() const { return inlierIndices; }
	const std::vector<int>& getOutlierIndices() const { return outlierIndices; }

	double getInliersAverageError() const {
		return pec->computeAverage(finalModel, inliers);
	}

	double getInliersRate() const {
		return inliersRate;
	}

	void setVerbose(bool v) {
		verbose = v;
	}

private:
	void randomlySampleN(const Samples& samples, Samples& nsamples, int sampleSize) {
		std::vector<int> is = ranis((int)samples.size(), sampleSize);
		for (int i = 0; i < sampleSize; i++) {
			nsamples.push_back(samples[is[i]]);
		}
	}

	/** from initial model, iterate to find the best model.
	 *
	 */
	M computeUntilConverge(M initModel, int initInliersNum, const Samples& samples) {
		if (verbose) {
			std::cout<<"iterate until converge...."<<std::endl;
			std::cout<<"init inliers number: "<<initInliersNum<<std::endl;
		}

		M bestModel = initModel;
		M newModel = initModel;

		int lastInliersNum = initInliersNum;

		Samples newInliers, newOutliers;
		pec->classify(initModel, samples, newInliers, newOutliers);
		double lastInlierAverageError = pec->computeAverage(initModel, newInliers);

		if (verbose) std::cout<<"init inlier average error: "<<lastInlierAverageError<<std::endl;

		while (true && (int)newInliers.size() >= modelSampleSize) {

			//update new model with new inliers, the new model does not necessarily have more inliers
			newModel = pmc->compute(newInliers);

			pec->classify(newModel, samples, newInliers, newOutliers);

			int newInliersNum = (int)newInliers.size();
			double newInlierAverageError = pec->computeAverage(newModel, newInliers);

			if (verbose) {
				std::cout<<"new inliers number: "<<newInliersNum<<std::endl;
				std::cout<<"new inlier average error: "<<newInlierAverageError<<std::endl;
			}
			if (newInliersNum < lastInliersNum) break;
			if (newInliersNum == lastInliersNum && newInlierAverageError >= lastInlierAverageError) break;

			//update best model with the model has more inliers
			bestModel = newModel;

			lastInliersNum = newInliersNum;
			lastInlierAverageError = newInlierAverageError;
		}

		return bestModel;
	}

	void clear() {
		inliers.clear();
		outliers.clear();
		inlierIndices.clear();
		outlierIndices.clear();
	}

	int rani(int max){ return rand() % max;	}

	//pick num points in range [0, max)
	std::vector<int> ranis(int max, int num){
		assert(max > 0);
		std::vector<int> is;
		if (num >= max) {
			for (int i = 0; i < max; i++) {
				is.push_back(i);
			}
		} else {
			for (int i = 0; i < num; ) {
				int v = rani(max);
				if (find(is.begin(), is.end(), v) == is.end()) {
					is.push_back(v);
					++i;
				}
			}
		}
		return is;
	}

private:
	GenericModelCalculator<M, S>* pmc;
	GenericErrorCaclculator<M, S>* pec;
	int modelSampleSize;

	int maxSampleCount;
	int minInliersNum;

	M finalModel;

	Samples inliers;
	Samples outliers;

	std::vector<int> inlierIndices;
	std::vector<int> outlierIndices;

	double inliersRate;

private:
	bool verbose;

};

}
#endif // FVISION_RANSAC_H_