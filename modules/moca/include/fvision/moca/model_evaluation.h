#ifndef FVISION_MODEL_EVALUATION_TEMPLATE_H_
#define FVISION_MODEL_EVALUATION_TEMPLATE_H_

#include <vector>

namespace fvision {

namespace moca_utils {

template <typename T>
T getMean(const std::vector<T>& vs) {
	int times = (int)vs.size();
	if (times == 0) return 0;
	T sum = 0;
	for (int i = 0; i < times; i++) { sum += vs[i];	}
	return sum / times;
}

template<typename T>
std::vector<T> getSubSet(const std::vector<T>& ts, const std::vector<int>& indices) {
	std::vector<T> nts(indices.size());
	for (size_t i = 0; i < indices.size(); i++) {
		nts[i] = ts[indices[i]];
	}
	return nts;
};

}

/// fit a model to a set of samples
/// M is the model type
/// S is the sample type
/// Interface:
///   M compute(samples); 
/// input a set of samples, output a model. 
/// after compute, the class should output inliers and outliers
/// 
template <typename M, typename S>
class GenericModelCalculator {
public:
	typedef std::vector<S> Samples;
	virtual M compute(const Samples& samples) = 0;

	virtual ~GenericModelCalculator<M, S>() {}

	//the model calculator may only use a subset of the samples for computing
	//default return empty for both
	virtual const std::vector<int>& getInlierIndices() const { return defaultInlierIndices; };
	virtual const std::vector<int>& getOutlierIndices() const { return defaultOutlierIndices; };

	// if the subclass has a threshold parameter, it need to override the following three functions
	// this is used for algorithms which have a normalization step on input samples
	virtual bool hasThreshold() const { return false; }
	virtual void setThreshold(double threshold) {}
	virtual double getThreshold() const { return 0; }

protected:
	std::vector<int> defaultInlierIndices;
	std::vector<int> defaultOutlierIndices;
};

/// evaluate a model to a set of samples
/// using a threshold to distinguish inliers and outliers
template <typename M, typename S>
class GenericErrorCaclculator {
public:
	virtual ~GenericErrorCaclculator<M, S>() {}

	typedef std::vector<S> Samples;

	virtual double compute(const M& model, const S& sample) const = 0;

	double computeAverage(const M& model, const Samples& samples) const {
		int n = (int)samples.size();
		if (n == 0) return 0;
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum += compute(model, samples[i]);
		}
		return sum / n;
	}

	double computeInlierAverage(const M& model, const Samples& samples) const {
		int n = (int)samples.size();
		if (n == 0) return 0;
		double sum = 0;
		double error = 0;
		int inlierNum = 0;
		for (int i = 0; i < n; i++) {
			error = compute(model, samples[i]);
			if (error <= threshold) {
				sum += error;
				inlierNum++;
			}
		}
		if (inlierNum == 0) return 1000000;
		return sum / inlierNum;
	}

public:

	/// set a threshold for classify inliers and outliers
	void setThreshold(double v) { threshold = v; }

	double getThreshold() const { return threshold; }

	/// classify all samples to inliers and outliers
	void classify(const M& model, const Samples& samples, Samples& inliers, Samples& outliers) const {
		inliers.clear();
		outliers.clear();
        typename Samples::const_iterator iter = samples.begin();
		for (; iter != samples.end(); ++iter) {
			if (isInlier(model, *iter)) inliers.push_back(*iter);
			else outliers.push_back(*iter);
		}
	}

	/// classify all samples to inliers and outliers, output indices
	void classify(const M& model, const Samples& samples, std::vector<int>& inlierIndices, std::vector<int>& outlierIndices) const {
		inlierIndices.clear();
		outlierIndices.clear();
        typename Samples::const_iterator iter = samples.begin();
		int i = 0;
		for (; iter != samples.end(); ++iter, ++i) {
			if (isInlier(model, *iter)) inlierIndices.push_back(i);
			else outlierIndices.push_back(i);
		}
	}

	/// classify all samples to inliers and outliers
	void classify(const M& model, const Samples& samples, 
		std::vector<int>& inlierIndices, std::vector<int>& outlierIndices, 
		Samples& inliers, Samples& outliers) const {

			inliers.clear();
			outliers.clear();
			inlierIndices.clear();
			outlierIndices.clear();
            typename Samples::const_iterator iter = samples.begin();
			int i = 0;
			for (; iter != samples.end(); ++iter, ++i) {
				if (isInlier(model, *iter)) {
					inliers.push_back(*iter);
					inlierIndices.push_back(i);
				}
				else {
					outliers.push_back(*iter);
					outlierIndices.push_back(i);
				}
			}
	}

	int calcInliersNumber(const M& model, const Samples& samples) const {
		int n = 0;
		for (int i = 0; i < (int)samples.size(); i++) {
			if (isInlier(model, samples[i])) ++n;
		}
		return n;
	}

	bool isInlier(const M& model, const S& sample) const {
		return (compute(model, sample) <= threshold);
	}

private:
	double threshold;
};

/// a container to hold model evaluation results
/// 
/// first set with samples and errors
/// then call filterWithThreshold
template <typename Sample>
class ModelEvaluationResults {
public:
	ModelEvaluationResults<Sample>() {}

public:
	typedef std::vector<Sample> Samples;
	typedef std::vector<double> Errors;
	typedef std::vector<int> Indices;

public:
	void filterWithThreshold(double threshold) {
		averageError = moca_utils::getMean(errors);
		getInlierOutlierIndices(errors, threshold, inlierIndices, outlierIndices);
		inlierErrors = moca_utils::getSubSet(errors, inlierIndices);
		outlierErrors = moca_utils::getSubSet(errors, outlierIndices);
		averageInlierError = moca_utils::getMean(inlierErrors);
		averageOutlierError = moca_utils::getMean(outlierErrors);
		inlierSamples = moca_utils::getSubSet(samples, inlierIndices);
		outlierSamples = moca_utils::getSubSet(samples, outlierIndices);
	}

	double getInliersRate() const {
		if (samples.size() == 0) return 0;
		return (double)inlierIndices.size() / samples.size();
	}

private:
	void getInlierOutlierIndices(const std::vector<double>& errors, double threshold, std::vector<int>& inlierIndices, std::vector<int>& outlierIndices) {
		inlierIndices.clear();
		outlierIndices.clear();
		for (int i = 0; i < (int)errors.size(); i++) {
			if (errors[i] <= threshold) inlierIndices.push_back(i);
			else outlierIndices.push_back(i);
		}
	}

public:
	Samples samples;
	Errors errors;
	double averageError;
	Indices inlierIndices;
	Indices outlierIndices;
	Errors inlierErrors;
	Errors outlierErrors;
	double averageInlierError;
	double averageOutlierError;
	Samples inlierSamples;
	Samples outlierSamples;

};

/// evaluate a model with a set of samples using specified error calculator
template <typename Model, typename Sample, typename ErrorCaculator>
class ModelEvaluator {
public:
	ModelEvaluator<Model, Sample, ErrorCaculator>(ErrorCaculator* errorCalculator, double threshold) {
		this->errorCalculator = errorCalculator;
		this->threshold = threshold;
	}

	ModelEvaluator<Model, Sample, ErrorCaculator>(double threshold) {
		this->errorCalculator = new ErrorCaculator();
		this->threshold = threshold;
	}

	~ModelEvaluator<Model, Sample, ErrorCaculator>() {
		delete errorCalculator;
	}

public:
	typedef std::vector<Sample> Samples;


	const ModelEvaluationResults<Sample>& compute(const Model& model, const Samples& samples) {
		results.samples = samples;
		results.errors.resize(samples.size());
		for (size_t i = 0; i < samples.size(); i++) {
			results.errors[i] = errorCalculator->compute(model, samples[i]);
		}
		results.filterWithThreshold(threshold);
		return results;
	}

public:
	ModelEvaluationResults<Sample> results;

private:
	ErrorCaculator* errorCalculator;
	double threshold;
};

}
#endif // FVISION_MODEL_EVALUATION_TEMPLATE_H_
