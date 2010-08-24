//compute point correspondences

static char usage[] = 
"\n pc version 1.3 \n\
\n\
usage: %s -L leftImage -R rightImage [options] \n\
\n\
reads two image file\n\
runs pc\n\
write result to xs.txt\n\
\n\
options:\n\
-f <pc.txt>    outpath point correspondences\n\
-d             draw correspondences images\n\
-c             write correspondences images to cos1.jpg and cos2.jpg\n\
-C <cos1.jpg>  output the cos1 image with a specified filepath\n\
-s             sift method\n\
-k             use pyramid LK method\n\
-o param       optical flow, parameters ('grid_size search_range patch_size')\n\
-p param       corner detector params ('haris 400 0.02 8' or 'grid 16')\n\
-m param       correspondences searching parameters ('half_win_size half_adjust_range max_difference max_search_range')\n\
-q             quiet (turn off debugging output)\n\
\
Example:\n\
//read left image and right image, output to 80-87.txt, use optical flow, set grid size 16, search range 20, patch size 10, write correspondences image\n\
pc_all -L im_0080.bmp -R im_0087.bmp -f 80-87.txt -o \"16 20 10\" -c\
\
output format\n\
n\n\
x1_1 y1_1\n\
x1_2 y1_2\n\
...\n\
n\n\
x2_1 y2_1\n\
x2_2 y2_2\n\
...\n\
\
(x1_1 y1_1) corresponds to (x2_1, y2_1)\n\
(x1_2 y1_2) corresponds to (x2_2, y2_2)\n\
\n";

#include <fvision.h>

#include <time.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <exception>

using namespace std;

using namespace fvision;

//-f
char* outpath = NULL;
//-q
bool verbose = true;
//-d
bool drawImages = false;
//-L
char* leftImage = NULL;
//-R
char* rightImage = NULL;
//-C
char* cos1Image = NULL;
//-c
bool writeImages = false;
//-o
bool opticalFlow = false;
int gridSize = 16;
int searchRange = 20;
int patchSize = 16;
//-s
bool sift = false;

//-p
int cornerDetectionMethod = 0; //0 for haris, 1 for grid
//for opencv
int cornerCount = 200;
double qualityLevel = 0.02;
int pointsMinDistance = 8;
//for grid
int blockSize = 16;

//-m
int halfWinSize = 4;
int halfAdjustRange = 3;
double maxDifference = 20;
double maxSearchRange = 50;

//-k
bool usePyrLK = false;

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "f:dL:R:cC:so:p:m:kq")) != EOF) {
		switch (c) {
			case 'f': outpath = argv[optind - 1]; break;
			case 'd': drawImages = true; break;
			case 'L': leftImage = argv[optind - 1]; break;
			case 'R': rightImage = argv[optind - 1]; break;
			case 'c': writeImages = true; break;
			case 'C': cos1Image = argv[optind - 1]; break;
			case 's': sift = true; break;
			case 'o': {
				opticalFlow = true;
				istringstream iss(argv[optind - 1]);
				iss>>gridSize>>searchRange>>patchSize;
				cout<<"grid size: "<<gridSize<<endl;
				cout<<"search range: "<<searchRange<<endl;
				cout<<"patch size: "<<patchSize<<endl;
				break;
					  }
			case 'p': {
				istringstream iss(argv[optind - 1]);
				char method[100];
				iss>>method;
				if (strcmp(method, "haris") == 0) {
					cornerDetectionMethod = 0;
					iss>>cornerCount>>qualityLevel>>pointsMinDistance;
				} else if (strcmp(method, "grid") == 0) {
					cornerDetectionMethod = 1;
					iss>>blockSize;
				} else {
					cout<<"error -p method! use default settings."<<endl;
				}
				break;
					  }
			case 'm': {
				istringstream iss(argv[optind - 1]);
				iss>>halfWinSize>>halfAdjustRange>>maxDifference>>maxSearchRange;
				break;
					  }
			case 'k': {
				usePyrLK = true;
					  }
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

	//load left image and right image
	fvision::exitIfNull(leftImage, "Please input left image with -L imagefile!");
	fvision::exitIfNull(rightImage, "Please input right image with -R imagefile!");

	IplImage* src = cvLoadImage(leftImage, 0);
	IplImage* src1 = cvLoadImage(rightImage, 0);

	fvision::exitIfNull(src, string("Cannot open left image: ") + leftImage + " !");
	fvision::exitIfNull(src1, string("Cannot open right image: ") + rightImage + " !");

	IplImage* csrc = cvLoadImage(leftImage, 1);
	IplImage* csrc1 = cvLoadImage(rightImage, 1);


	//compute point correspondences
	vector<CvPoint2D32f> x1s, x2s;
	long s = GetTickCount();
	TwoViewPointTrackerFactory factory;
	TwoViewPointTracker* tracker = NULL;
	if (sift) {
		tracker = factory.createSIFT();
	} else if (opticalFlow) {
		tracker = factory.createBlockTracker(gridSize, searchRange, patchSize);
	} else {
		tracker = factory.createPyrKLT(cornerCount, qualityLevel, pointsMinDistance);
	}
	tracker->compute(src, src1, x1s, x2s);
	long s2 = GetTickCount();
	cout<<"time: "<<s2 - s<<" milliseconds........"<<endl;

	PointPairs pps;
	composePointPairs(pps, x1s, x2s);

	//save results
	if (outpath != NULL) {
		try {
			save(outpath, pps);
		} catch (std::exception& e) {
			cout<<e.what()<<endl;
			exit(1);
		} catch (...) {	
			cout<<"error to write file: "<<outpath<<endl;
			exit(1);
		}
	}

	if (drawImages || writeImages || cos1Image) {
		PointCorrespondencesDrawer pcDrawer;

		IplImage* cos1 = pcDrawer.create(csrc, pps);
		IplImage* cos2 = pcDrawer.create(csrc, csrc1, pps);

		if (cos1Image) {
			cvSaveImage(cos1Image, cos1);
		}

		if (drawImages) {
			showImage("cos1", cos1);
			showImage("cos2", cos2);
		}
		if (writeImages) {
			cvSaveImage("cos1.jpg", cos1);
			cvSaveImage("cos2.jpg", cos2);
		}
		cvReleaseImage(&cos1);
		cvReleaseImage(&cos2);
	}

	cvReleaseImage(&src);
	cvReleaseImage(&src1);
	cvReleaseImage(&csrc);
	cvReleaseImage(&csrc1);

	cvWaitKey(0);

    return 0;
}
