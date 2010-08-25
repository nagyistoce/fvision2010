#ifndef FVISION_POINT_RECONSTRUCTOR_H_
#define FVISION_POINT_RECONSTRUCTOR_H_


#include <cv.h>
#include <vector>

#include <fvision/geom.h>

namespace fvision {

class PointReconstructionResults {

public:
	void filterWithThreshold(double thresholdOfReprojectionError);

	double getInliersRate() const { 
		return (double)inlierIndices.size() / Xs.size(); 
	}

	PointPairs getInliers() const;

	PointPairs getOutliers() const;

	PointPairs getOriginalPointPairs() const;

	void denormalize(const CvMat* K1, const CvMat* K2);

public:
	std::vector<CvPoint2D32f> x1s;
	std::vector<CvPoint2D32f> x2s;

	std::vector<CvPoint3D64f> Xs;
	std::vector<double> reprojectionErrors;


	std::vector<int> inlierIndices;
	std::vector<int> outlierIndices;

	double averageInliersReprojectionError;
	double averageReprojectionError;


	std::vector<CvPoint3D64f> iXs;

	//inliers
	std::vector<CvPoint2D32f> ix1s;
	std::vector<CvPoint2D32f> ix2s;

	//outliers
	std::vector<CvPoint2D32f> ox1s;
	std::vector<CvPoint2D32f> ox2s;

	//back projection of Xs
	std::vector<CvPoint2D32f> bx1s;
	std::vector<CvPoint2D32f> bx2s;	

	//inliers of back projection
	std::vector<CvPoint2D32f> ibx1s;
	std::vector<CvPoint2D32f> ibx2s;

	//outliers of back projection
	std::vector<CvPoint2D32f> obx1s;
	std::vector<CvPoint2D32f> obx2s;

};

/**
 * reconstruct 3D points with two cameras and image points
 * separate inliers and outliers based on re-projection error
 */
class PointReconstructor {
public:
	PointReconstructor(double thresholdOfReprojectionError);
	~PointReconstructor(void);

public:
	void compute(const CvMat* P1, 
		         const CvMat* P2, 
				 const std::vector<CvPoint2D32f>& x1s, 
				 const std::vector<CvPoint2D32f>& x2s);

public:
	void setThresholdOfReprojectionError(double threshold) {
		this->thresholdOfReprojectionError = threshold;
	}

public:
	PointReconstructionResults results;

private:
	double thresholdOfReprojectionError;
};

}
#endif // FVISION_POINT_RECONSTRUCTOR_H_