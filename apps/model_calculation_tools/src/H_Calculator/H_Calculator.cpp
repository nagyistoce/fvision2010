//input two images and point correspondences, compute homography that transform the second image to the first one
//transform the second image, overlay on the first image

static char usage[] = 
"\n H_Calculator version 2.2 \n\
\n\
usage: %s -i input [options] \n\
\n\
reads point corresondences file\n\
runs homography estimation\n\
write homography and overlay image\n\
\n\
options:\n\
-o dir         output dir\n\
-i input       input file, point correspondences \n\
-t threshold   threshold \n\
-f filaname    write homography matrix to output file\n\
-L left_image  left image\n\
-R right_image right image\n\
-d             draw intemediate images\n\
-s             save intemediate images to icos1.jpg, icos2.jpg, icos1.jpg, ocos2.jpg, im1_h.jpg, overlay.jpg\n\
-P prefix      set prefix of output file names\n\
-e extension   set extension of output image files\n\
-q             quiet (turn off debugging output)\n\n";

#include <cv.h>
#include <highgui.h>

#include <fvision/calculators.h>
#include <fvision/langutils.h>
#include <fvision/cvdraw.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;
using namespace fvision;

//-i
char* input = NULL;
//-t
double threshold = 1.5;
//-f
char* h_filename = "H.txt";
//-q
bool verbose = true;
//-d
bool drawImages = false;
//-L
char* leftImage = NULL;
//-R
char* rightImage = NULL;
//-s
bool saveImages = false;
//-P
string prefix;
//-e
string extension = "jpg";
//-o
string outputDir;

void checkInputs() {
	if (input == NULL) {
		cout<<"please input point correspondences file using '-i filename'!"<<endl;
		exit(1);
	}
}

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "o:i:t:f:dL:R:sP:e:q")) != EOF) {
		switch (c) {
			case 'o': outputDir = argv[optind - 1]; break;
			case 'i': input = argv[optind - 1]; break;
			case 't': threshold = atof(argv[optind - 1]); break;
			case 'f': h_filename = argv[optind - 1]; break;
			case 'd': drawImages = true; break;
			case 'L': leftImage = argv[optind - 1]; break;
			case 'R': rightImage = argv[optind - 1]; break;
			case 's': saveImages = true; break;
			case 'P': prefix     = argv[optind - 1]; break;
			case 'e': extension  = argv[optind - 1]; break;
			case 'q': verbose = false; break;
			default:
				cerr<<"Ignoring unrecognized option "<<argv[optind-1]<<endl;
		}
	}
	if (argc == 1 || optind != argc) {
		printf(usage, argv[0]);
		exit(1);
	}
}

int main(int argc, char* argv[]) {

	processCommandLine(argc, argv);

	checkInputs();

	PointPairs pps;
	ifstream pcifs(input);
	pcifs>>pps;
	pcifs.close();

	fvision::exitIf(pps.size() == 0, string("can not get point correspondences from the input file: ") + input + " !");
	fvision::exitIf(pps.size() < 5, "at least 5 points should be input!");

	HomographyCalculator* ransac = HomographyCalculatorFactory().createRansac(threshold);

	if (verbose) { cout<<"threshold: "<<threshold<<endl; }

	CvMatWrapper H = ransac->compute(pps);

	if (verbose) { cout<<"H: "<<endl<<H<<endl; }

	if (h_filename != NULL) { save(H.getMat(), h_filename); }

	HomographyGeometricErrorEvaluator* fmec = new HomographyGeometricErrorEvaluator(threshold);
	HomographyEvaluationResults fResults = fmec->compute(H.getMat(), pps);

	if (verbose) {
		cout<<"inliers error: "<<fResults.averageInlierError<<endl;
		cout<<"inliers rate:"<<fResults.getInliersRate()<<endl;
	}

	PointPairs ipps = fResults.inlierSamples;
	PointPairs opps = fResults.outlierSamples;

	PointCorrespondencesDrawer pcDrawer;

	if (drawImages || saveImages) {

		fvision::exitIfNull(leftImage, "Please input left image with -L imagefile!");
		fvision::exitIfNull(rightImage, "Please input right image with -R imagefile!");

		IplImage* im1 = cvLoadImage(leftImage, 1);

		fvision::exitIfNull(im1, string("Cannot open left image: ") + leftImage + " !");

		IplImage* im2 = cvLoadImage(rightImage, 1);

		fvision::exitIfNull(cout, string("Cannot open right image: ") + rightImage + " !");

		IplImage* cos1 = pcDrawer.create(im1, pps);
		IplImage* cos2 = pcDrawer.create(im1, im2, pps);

		IplImage* icos1 = pcDrawer.create(im1, ipps);
		IplImage* icos2 = pcDrawer.create(im1, im2, ipps);

		IplImage* ocos1 = pcDrawer.create(im1, opps);
		IplImage* ocos2 = pcDrawer.create(im1, im2, opps);

		IplImage* im1_h = perspectiveWarp(im1, H.getMat());
		IplImage* im_overlay = mix(im1_h, im2);

		vector<CvPoint2D32f> ix1s, ix2s;
		decomposePointPairs(ipps, ix1s, ix2s);
		vector<CvPoint2D32f> ox1s, ox2s;
		decomposePointPairs(opps, ox1s, ox2s);
		PointDrawer ipd("+", Colors::GREEN);
		drawPoints(im1, ix1s, ipd);
		drawPoints(im2, ix2s, ipd);
		PointDrawer opd("+", Colors::RED);
		drawPoints(im1, ox1s, opd);
		drawPoints(im2, ox2s, opd);

		ImageDemoHelper demoHelper;
		demoHelper.setDir(outputDir);
		demoHelper.setShow(drawImages);
		demoHelper.setSave(saveImages);
		demoHelper.setPrefix(prefix);
		demoHelper.setExtension(extension);

		demoHelper.demo(cos1, "cos1");
		demoHelper.demo(cos2, "cos2");
		demoHelper.demo(icos1, "icos1");
		demoHelper.demo(icos2, "icos2");
		demoHelper.demo(ocos1, "ocos1");
		demoHelper.demo(ocos2, "ocos2");
		demoHelper.demo(im1_h, "im1_h");
		demoHelper.demo(im_overlay, "overlay");

		cvReleaseImage(&cos1);
		cvReleaseImage(&cos2);
		cvReleaseImage(&icos1);
		cvReleaseImage(&im1);
		cvReleaseImage(&im2);
		cvReleaseImage(&im1_h);
		cvReleaseImage(&im_overlay);

	}

	cvWaitKey(0);

    return 0;
}
