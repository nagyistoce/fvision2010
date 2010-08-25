#include "TwoViewReconstructor.h"

#include <fvision/geom.h>

#include <fvision/moca.h>

namespace fvision {

using namespace moca_utils;

std::vector<CvPoint2D32f> TwoViewReconstructionResults::getFInliersOnLeftImage() const {
	return getSubSet(pointResults.x1s, fInlierIndices);
}

std::vector<CvPoint2D32f> TwoViewReconstructionResults::getFInliersOnRightImage() const {
	return getSubSet(pointResults.x2s, fInlierIndices);
}

std::vector<CvPoint2D32f> TwoViewReconstructionResults::getFOutliersOnLeftImage() const {
	return getSubSet(pointResults.x1s, fOutlierIndices);
}

std::vector<CvPoint2D32f> TwoViewReconstructionResults::getFOutliersOnRightImage() const {
	return getSubSet(pointResults.x2s, fOutlierIndices);
}

fvision::PointPairs TwoViewReconstructionResults::getFInliers() const {
	return getSubSet(pointResults.getOriginalPointPairs(), fInlierIndices);
}

fvision::PointPairs TwoViewReconstructionResults::getFOutliers() const {
	return getSubSet(pointResults.getOriginalPointPairs(), fOutlierIndices);
}

TwoViewReconstructionResults* TwoViewReconstructionResults::getDenormalizedResults(const CvMat* K1, const CvMat* K2) const {
	TwoViewReconstructionResults* nResults = new TwoViewReconstructionResults();
	*nResults = *this;
	nResults->fw = getFFromEK(fw.getMat(), K1, K2);
	nResults->cameras[0].setK(K1);
	nResults->cameras[1].setK(K2);
	//nResults->pointResults = pointResults;
	nResults->pointResults.denormalize(K1, K2);
	//nResults->fInlierIndices = fInlierIndices;
	//nResults->fOutlierIndices = fOutlierIndices;
	return nResults;
}
}

