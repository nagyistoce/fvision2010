#include "TwoViewReconstructionDemo.h"

#include <fvision/geom.h>
#include <fvision/calculators.h>
#include <fvision/cvdraw.h>

using namespace std;

namespace fvision {


TwoViewReconstructionDemo::TwoViewReconstructionDemo(
	const TwoViewReconstructionResults& tvrResults, 
	const IplImage* leftImage, 
	const IplImage* rightImage) : results (tvrResults)
{

	assert(leftImage != NULL && rightImage != NULL);

	this->leftImage = cvCloneImage(leftImage);
	this->rightImage = cvCloneImage(rightImage);

	leftEpipolarLinesImage   = cvCloneImage(leftImage);
	rightEpipolarLinesImage  = cvCloneImage(rightImage);

	leftBackProjectionImage  = cvCloneImage(leftImage);
	rightBackProjectionImage = cvCloneImage(rightImage);

	leftFInlierOutlierImage  = cvCloneImage(leftImage);
	rightFInlierOutlierImage = cvCloneImage(rightImage);

	PointCorrespondencesDrawer pcDrawer;
	twoViewPcImage = pcDrawer.create(leftImage, rightImage, results.pointResults.x1s, results.pointResults.x2s);
	oneViewPcImage = pcDrawer.create(leftImage, results.pointResults.x1s, results.pointResults.x2s);

	EpipolarLinesDrawer epDrawer;
	epDrawer.draw(results.fw.getMat(), leftEpipolarLinesImage, rightEpipolarLinesImage);

	rightWarpImage  = cvCloneImage(rightImage);

	CvMat* M1 = tvrResults.cameras[0].getM();
	CvMat* M2 = tvrResults.cameras[1].getM();
	CvMat* invM2 = inv(M2);
	CvMat* H21 = matMul(M1, invM2);
	cvWarpPerspective(rightEpipolarLinesImage, rightWarpImage, H21);
	warpOverlayImage = mix(leftEpipolarLinesImage, rightWarpImage);
	vector<CvPoint2D32f> x21s = transformPoints(H21, results.pointResults.x2s);
	pcDrawer.draw(warpOverlayImage, results.pointResults.x1s, x21s);
	clearMats(&M1, &M2, &invM2, &H21);


	inlierPointDrawer = PointDrawer("o", Colors::GREEN);
	outlierPointDrawer = PointDrawer("o", Colors::RED);
	pcPointDrawer = PointDrawer("+", Colors::GREEN);

	backProjectionPointDrawer = PointDrawer("+", Colors::GREEN);

}

TwoViewReconstructionDemo::~TwoViewReconstructionDemo() {
	cvReleaseImage(&leftImage);
	cvReleaseImage(&rightImage);
	cvReleaseImage(&leftEpipolarLinesImage);
	cvReleaseImage(&rightEpipolarLinesImage);
	cvReleaseImage(&leftBackProjectionImage);
	cvReleaseImage(&rightBackProjectionImage);
	cvReleaseImage(&oneViewPcImage);
	cvReleaseImage(&twoViewPcImage);
	cvReleaseImage(&leftFInlierOutlierImage);
	cvReleaseImage(&rightFInlierOutlierImage);
}

void TwoViewReconstructionDemo::demo(const ImageDemoHelper& imageDemo) {

	drawPoints(leftBackProjectionImage, results.pointResults.ix1s, inlierPointDrawer);
	drawPoints(rightBackProjectionImage, results.pointResults.ix2s, inlierPointDrawer);

	drawPoints(leftBackProjectionImage, results.pointResults.ox1s, outlierPointDrawer);
	drawPoints(rightBackProjectionImage, results.pointResults.ox2s, outlierPointDrawer);

	drawPoints(leftBackProjectionImage, results.pointResults.bx1s, backProjectionPointDrawer);
	drawPoints(rightBackProjectionImage, results.pointResults.bx2s, backProjectionPointDrawer);

	drawPoints(leftFInlierOutlierImage, results.getFInliersOnLeftImage(), inlierPointDrawer);
	drawPoints(rightFInlierOutlierImage, results.getFInliersOnRightImage(), inlierPointDrawer);

	drawPoints(leftFInlierOutlierImage, results.getFOutliersOnLeftImage(), outlierPointDrawer);
	drawPoints(rightFInlierOutlierImage, results.getFOutliersOnRightImage(), outlierPointDrawer);

	PointCorrespondencesDrawer pcDrawer;
	IplImage* outlierTwoViewPcImage = pcDrawer.create(leftImage, rightImage, results.pointResults.ox1s, results.pointResults.ox2s);
	IplImage* outlierOneViewPcImage = pcDrawer.create(leftImage, results.pointResults.ox1s, results.pointResults.ox2s);

	imageDemo.demo(outlierOneViewPcImage, "outlierOneViewPcImage");
	imageDemo.demo(outlierTwoViewPcImage, "outlierTwoViewPcImage");

	imageDemo.demo(oneViewPcImage,     "oneViewPcImage"   );
	imageDemo.demo(twoViewPcImage,    "twoViewPcImage"  );
	imageDemo.demo(leftEpipolarLinesImage,     "leftEpipolarLines"   );
	imageDemo.demo(rightEpipolarLinesImage,    "rightEpipolarLines"  );
	imageDemo.demo(leftBackProjectionImage,     "leftBackProjection"   );
	imageDemo.demo(rightBackProjectionImage,    "rightBackProjection"  );
	imageDemo.demo(leftFInlierOutlierImage,     "leftFInlierOutlier"   );
	imageDemo.demo(rightFInlierOutlierImage,    "rightFInlierOutlier"  );

	imageDemo.demo(rightWarpImage, "rightWarpImage");
	imageDemo.demo(warpOverlayImage, "warpOverlayImage");

	HomographyCalculatorFactory::Configuration config;
	config.ransacDistanceThreshold = 3.0;
	HomographyCalculator* hc = HomographyCalculatorFactory().create(config);
	PointPairs pps;
	composePointPairs(pps, results.pointResults.ix2s, results.pointResults.ix1s);
	CvMatWrapper H = hc->compute(pps);
	HomographyEvaluationResults evalResults = HomographyGeometricErrorEvaluator(new HomographyGeometricErrorCalculator(), 3.0).compute(H, pps);
	cout<<"homography inliers rate: "<<evalResults.getInliersRate()<<endl;

	IplImage* rightWarpImage2 = cvCloneImage(rightImage);
	cvWarpPerspective(rightEpipolarLinesImage, rightWarpImage2, H.getMat());
	IplImage* warpOverlayImage2 = mix(leftEpipolarLinesImage, rightWarpImage2);
	vector<CvPoint2D32f> x21s = transformPoints(H.getMat(), results.pointResults.x2s);
	pcDrawer.draw(warpOverlayImage2, results.pointResults.x1s, x21s);
	drawPoints(warpOverlayImage2, moca_utils::getSubSet(results.pointResults.ix1s, evalResults.inlierIndices), PointDrawer("o"));
	drawPoints(warpOverlayImage2, moca_utils::getSubSet(results.pointResults.ix1s, evalResults.outlierIndices), PointDrawer("o", Colors::RED));
	imageDemo.demo(rightWarpImage2, "rightWarpImage2");
	imageDemo.demo(warpOverlayImage2, "warpOverlayImage2");


}

}