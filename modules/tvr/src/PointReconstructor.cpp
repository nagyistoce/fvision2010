#include "PointReconstructor.h"

#include <fvision/moca.h>
#include <fvision/geom.h>

using namespace std;

namespace fvision {

using namespace moca_utils;

PointReconstructor::PointReconstructor(double thresholdOfReprojectionError) {
	this->thresholdOfReprojectionError = thresholdOfReprojectionError;
}

PointReconstructor::~PointReconstructor(void)
{
}

void PointReconstructor::compute(const CvMat* P1, 
								   const CvMat* P2, 
								   const std::vector<CvPoint2D32f>& x1s, 
								   const std::vector<CvPoint2D32f>& x2s)
{
	
	results.x1s = x1s;
	results.x2s = x2s;

	results.Xs = triangulateToPoint3Ds(P1, P2, results.x1s, results.x2s);
	results.reprojectionErrors = triangulationErrors(results.Xs, P1, P2, results.x1s, results.x2s);

	results.bx1s = projectWithP(P1, results.Xs);
	results.bx2s = projectWithP(P2, results.Xs);

	results.filterWithThreshold(thresholdOfReprojectionError);

}

namespace pr_internal {
	void getInlierOutlierIndices(const std::vector<double>& errors, double threshold, std::vector<int>& inlierIndices, std::vector<int>& outlierIndices) {
		inlierIndices.clear();
		outlierIndices.clear();
		for (int i = 0; i < (int)errors.size(); i++) {
			if (errors[i] <= threshold) inlierIndices.push_back(i);
			else outlierIndices.push_back(i);
		}
	}
}

void PointReconstructionResults::filterWithThreshold(double thresholdOfReprojectionError) {
	using namespace pr_internal;
	getInlierOutlierIndices(reprojectionErrors, thresholdOfReprojectionError, inlierIndices, outlierIndices);

	vector<double> inlierErrors = getSubSet(reprojectionErrors, inlierIndices);

	averageInliersReprojectionError = getMean(inlierErrors);
	averageReprojectionError = getMean(reprojectionErrors);

	iXs  = getSubSet(Xs, inlierIndices);

	ix1s = getSubSet(x1s, inlierIndices);
	ix2s = getSubSet(x2s, inlierIndices);

	ox1s = getSubSet(x1s, outlierIndices);
	ox2s = getSubSet(x2s, outlierIndices);

	ibx1s = getSubSet(bx1s, inlierIndices);
	ibx2s = getSubSet(bx2s, inlierIndices);

	obx1s = getSubSet(bx1s, outlierIndices);
	obx2s = getSubSet(bx2s, outlierIndices);

}

PointPairs PointReconstructionResults::getInliers() const {
	PointPairs pps;
	composePointPairs(pps, ix1s, ix2s);
	return pps;
}

PointPairs PointReconstructionResults::getOutliers() const {
	PointPairs pps;
	composePointPairs(pps, ox1s, ox2s);
	return pps;
}

fvision::PointPairs PointReconstructionResults::getOriginalPointPairs() const {
	PointPairs pps;
	composePointPairs(pps, x1s, x2s);
	return pps;	
}

namespace pr_internal {
	template <typename T, typename Transformer>
	void modifyTransform(std::vector<T>& vs, const Transformer* transformer) {
		for (size_t i = 0; i < vs.size(); i++) {
			vs[i] = transformer->transform(vs[i]);
		}
	}
}

void PointReconstructionResults::denormalize(const CvMat* K1, const CvMat* K2) {

	using namespace fvision::point2d_transformers;
	using namespace fvision::pr_internal;

	Point2DTransformer<CvPoint2D32f>* pt1 = kDenormalizer<CvPoint2D32f>(K1);
	modifyTransform(x1s, pt1);
	modifyTransform(ix1s, pt1);
	modifyTransform(ox1s, pt1);
	modifyTransform(bx1s, pt1);
	modifyTransform(ibx1s, pt1);
	modifyTransform(obx1s, pt1);
	delete pt1;

	Point2DTransformer<CvPoint2D32f>* pt2 = kDenormalizer<CvPoint2D32f>(K2);
	modifyTransform(x2s, pt2);
	modifyTransform(ix2s, pt2);
	modifyTransform(ox2s, pt2);
	modifyTransform(bx2s, pt2);
	modifyTransform(ibx2s, pt2);
	modifyTransform(obx2s, pt2);
	delete pt2;

}
}