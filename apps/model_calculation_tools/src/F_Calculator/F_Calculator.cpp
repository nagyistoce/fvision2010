//compute fundamental matrix based on point correspondences

static char usage[] = 
"\n FCalculator version 1.3 \n\
\n\
usage: %s -i input [options] \n\
\n\
reads point correspondences file\n\
runs FCalculator\n\
write inlier point correspondences to ixs.txt and outliers to oxs.txt\n\
the inliers and outliers is sperated by the distance of point to epipolar line\n\
if the distance is less than 1.5, then it's an inlier, otherwise an outlier\n\
\n\
options:\n\
-o dir         output directory (default current directory)\n\
-i input       input file, point correspondences \n\
-t threshold   threshold (default 1.5) \n\
-h             Homography based method\n\
-f filaname    write fundamental matrix to output file\n\
-d             draw correspondences images, epipolar lines images\n\
-L left_image  left image\n\
-R right_image right image\n\
-s             save intermediate images\n\
-q             quiet (turn off debugging output)\n\n";

#include <iostream>
#include <fstream>

#include <fvision/langutils.h>
#include <fvision/calculators.h>
#include <fvision/cvdraw.h>

#include <stdlib.h>

using namespace std;
using namespace fvision;

//-i
char* input = NULL;
//-t
double threshold = 1.5;
//-n
char* inputDir = ".";
//-o
char* outputDir = ".";
//-h
bool homographyBased = false;
//-f
char* f_filename = NULL;
//-q
bool verbose = true;
//-d
bool drawImages = false;
//-L
char* leftImage = NULL;
//-R
char* rightImage = NULL;
//-s
bool saveIntermediateImages = false;

PointPairs pps;
FileOutputHelper outputHelper;
FundamentalMatrixCalculator* pfmc = NULL;

char infilepath[300];
char outfilepath[300];

bool checkInputs() {
	if (input == NULL) {
		cout<<"please input point correspondences file using '-i filename'!"<<endl;
		return false;
	}
	return true;
}

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "n:o:i:t:hf:dL:R:sq")) != EOF) {
		switch (c) {
			case 'n': inputDir = argv[optind - 1]; break;
			case 'o': outputDir = argv[optind - 1]; break;
			case 'i': input = argv[optind - 1]; break;
			case 't': threshold = atof(argv[optind - 1]); break;
			case 'h': homographyBased = true; break;
			case 'f': f_filename = argv[optind - 1]; break;
			case 'd': drawImages = true; break;
			case 'L': leftImage = argv[optind - 1]; break;
			case 'R': rightImage = argv[optind - 1]; break;
			case 's': saveIntermediateImages = true; break;
			case 'q': verbose = false; break;
			default:
				cerr<<"Ignoring unrecognized option "<<argv[optind-1]<<endl;
		}
	}
	if (optind != argc || !checkInputs()) {
		printf(usage, argv[0]);
		exit(1);
	}
}

void analyzeF(const CvMat* F, CvSize imSize) {
	CvMat* K = fvision::getKFromVerticalFovAndImageSize(40 * CV_PI / 180, imSize);
	CvMat* e1 = null(F);
	CvMat* e2 = leftNull(F);

	double xtheta, ztheta;
	getEpipoleThetas(e1, K, ztheta, xtheta);
	cout<<"e1 thetas: "<<radianToDegrees(ztheta)<<" - "<<radianToDegrees(xtheta)<<endl;
	getEpipoleThetas(e2, K, ztheta, xtheta);
	cout<<"e2 thetas: "<<radianToDegrees(ztheta)<<" - "<<radianToDegrees(xtheta)<<endl;

}

void evaluateF(const CvMat* F, const PointPairs& pps) {

	FundamentalMatrixGeometricErrorEvaluator* fmec = new FundamentalMatrixGeometricErrorEvaluator(new FundamentalMatrixGeometricErrorCalculator(), threshold);
	FundamentalMatrixEvaluationResults fResults = fmec->compute(F, pps);

	if (verbose) {
		cout<<"inliers error: "<<fResults.averageInlierError<<endl;
		cout<<"inliers rate:"<<fResults.getInliersRate()<<endl;
	}

	outputHelper.save("ixs", fResults.inlierSamples);
	outputHelper.save("oxs", fResults.inlierSamples);

	PointCorrespondencesDrawer pcDrawer;

	if (drawImages || saveIntermediateImages) {

		fvision::exitIfNull(leftImage, "Please input left image with -L imagefile!");
		fvision::exitIfNull(rightImage, "Please input right image with -R imagefile!");

		IplImage* im1 = cvLoadImage(leftImage, 1);

		fvision::exitIfNull(im1, string("Cannot open left image: ") + outfilepath + " !");

		IplImage* im2 = cvLoadImage(rightImage, 1);

		fvision::exitIfNull(cout, string("Cannot open right image: ") + outfilepath + " !");

		IplImage* cos1 = pcDrawer.create(im1, pps);
		IplImage* cos2 = pcDrawer.create(im1, im2, pps);

		IplImage* icos1 = pcDrawer.create(im1, fResults.inlierSamples);
		IplImage* icos2 = pcDrawer.create(im1, im2, fResults.inlierSamples);

		IplImage* ocos1 = pcDrawer.create(im1, fResults.outlierSamples);
		IplImage* ocos2 = pcDrawer.create(im1, im2, fResults.outlierSamples);

		EpipolarLinesDrawer elDrawer;
		elDrawer.draw(F, im1, im2);

		ImageDemoHelper demoHelper;
		demoHelper.setDir(outputDir);
		demoHelper.setShow(drawImages);
		demoHelper.setSave(saveIntermediateImages);

		demoHelper.demo(cos1, "cos1");
		demoHelper.demo(cos2, "cos2");
		demoHelper.demo(icos1, "icos1");
		demoHelper.demo(icos2, "icos2");
		demoHelper.demo(ocos1, "ocos1");
		demoHelper.demo(ocos2, "ocos2");
		demoHelper.demo(im1, "eplines1");
		demoHelper.demo(im2, "eplines2");

		CvSize imSize = cvGetSize(im1);
		analyzeF(F, imSize);

		//show overlay homography warped image
		//show h inliers in green, e inliers in yellow, and e outliers in red
		//show 
/*		if (homographyBased) {
			NormalizedFundamentalMatrixCalculator* npfmc = (NormalizedFundamentalMatrixCalculator*)pfmc;
			HomographyBasedFundamentalMatrixCalculator* hF = static_cast<HomographyBasedFundamentalMatrixCalculator*>(npfmc->getModelCalculator());
			IplImage* fim1 = cvCloneImage(im1);
			pcDrawer.setPointDrawer(PointDrawer("o", Colors::GREEN));
			pcDrawer.draw(fim1, getSubSet(pps, hF->initialFInlierIndices));
			pcDrawer.setPointDrawer(PointDrawer("o", Colors::RED));
			pcDrawer.draw(fim1, getSubSet(pps, hF->initialFInlierIndices));
			demoHelper.demo(fim1, "finlieroutlers");
			cout<<"initial f inliers rate: "<<hF->initialFInliersRate<<endl;
			IplImage* him1 = perspectiveWarp(im1, hF->H.getMat());
			IplImage* overlay = mix(him1, im2);
			pcDrawer.setPointDrawer(PointDrawer("o", Colors::GREEN));
			pcDrawer.draw(overlay, hF->hInliers);
			pcDrawer.setPointDrawer(PointDrawer("o", Colors::YELLOW));
			pcDrawer.draw(overlay, hF->eInliers);
			pcDrawer.setPointDrawer(PointDrawer("o", Colors::RED));
			pcDrawer.draw(overlay, hF->eOutliers);
			demoHelper.demo(overlay, "Homography_based");
			cvReleaseImage(&him1);
			cvReleaseImage(&overlay);
		}
*/
		cvReleaseImage(&cos1);
		cvReleaseImage(&cos2);
		cvReleaseImage(&icos1);
		cvReleaseImage(&icos2);
		cvReleaseImage(&ocos1);
		cvReleaseImage(&ocos2);
		cvReleaseImage(&im1);
		cvReleaseImage(&im2);

		cvWaitKey(0);
	}

}

void setParameters() {
	input = "..\\..\\data\\endoscopy\\pc.txt";
	leftImage = "..\\..\\data\\endoscopy\\im_0024.bmp";
	rightImage = "..\\..\\data\\endoscopy\\im_0029.bmp";
	drawImages = true;
	homographyBased = true;
	threshold = 1.5;
}

int main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();

	load(input, pps);

	fvision::exitIf(pps.size() == 0, string("can not get point correspondences from the input file: ") + infilepath + " !");
	fvision::exitIf(pps.size() < 8, "at least 8 points should be input!");

	FundamentalMatrixCalculatorFactory::Configuration config;
	if (homographyBased) config.method = FundamentalMatrixCalculatorFactory::Configuration::HOMOGRAPHY_BASED;
	config.ransacDistanceThreshold = threshold;
	if (verbose) { cout<<"configuration: "<<endl<<config<<endl; }

	pfmc = FundamentalMatrixCalculatorFactory().create(config);

	CvMatWrapper fw = pfmc->compute(pps);
	const CvMat* F = fw.getMat();

	outputHelper.setDir(outputDir);
	outputHelper.setExtension("txt");

	if (verbose) { cout<<"F: "<<endl<<F<<endl; }

	if (f_filename != NULL) { outputHelper.save(f_filename, fw); }

	evaluateF(F, pps);

    return 0;
}
