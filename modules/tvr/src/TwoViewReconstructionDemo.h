#pragma once

#include "TwoViewReconstructor.h"
#include <fvision/cvdraw.h>

namespace fvision {

/** Demo two view reconstruction results
 * 
 */
class TwoViewReconstructionDemo {
public:
	TwoViewReconstructionDemo(
		const TwoViewReconstructionResults& tvrResults, 
		const IplImage* leftImage, 
		const IplImage* rightImage);

	~TwoViewReconstructionDemo();

private:
	//TwoViewReconstructionDemo(const TwoViewReconstructionDemo& rhs) {}
	//TwoViewReconstructionDemo& operator=(const TwoViewReconstructionDemo& rhs) {}

public:
	void demo(const ImageDemoHelper& imageDemo);

public:
	IplImage* leftEpipolarLinesImage;
	IplImage* rightEpipolarLinesImage;

	IplImage* oneViewPcImage;
	IplImage* twoViewPcImage;

	IplImage* leftBackProjectionImage;
	IplImage* rightBackProjectionImage;

	IplImage* leftFInlierOutlierImage;
	IplImage* rightFInlierOutlierImage;

	IplImage* rightWarpImage;
	IplImage* warpOverlayImage;

	const TwoViewReconstructionResults& results;

public:
	PointDrawer backProjectionPointDrawer;
	PointDrawer inlierPointDrawer;
	PointDrawer outlierPointDrawer;

	PointDrawer pcPointDrawer;

private:
	IplImage* leftImage;
	IplImage* rightImage;

};

}