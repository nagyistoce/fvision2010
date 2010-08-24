//input left image, right image, point correspondences
//show full point correspondences images, cos1 and cos2
//a slide bar to control points to be checked
//draw crosses or boxes on selected points on both images
//zome in 7x7 window on selected points

static char usage[] = 
"\npc_view version 1.4 \n\
\n\
usage: %s -L <left_image> -R <right_image> -i <pc.txt> [options] \n\
\n\
reads left image, right image and point corresondences file\n\
runs pc view\n\
shows correspondences\n\
\n\
options:\n\
-q             quiet (turn off debugging output)\n\n";

#include <fvision/langutils.h>
#include <fvision/cvdraw.h>
#include <fvision/geom.h>

#include <highgui.h>
#include <iostream>
#include <fstream>

using namespace fvision;
using namespace std;

//-i
char* input = NULL;
//-L
char* leftImage = NULL;
//-R
char* rightImage = NULL;

IplImage* im1 = NULL;
IplImage* im2 = NULL;
PointPairs pc;

PointCorrespondencesDrawer pcDrawer;

int selPointIndex = 0;

void onSelPointIndexChange(int pos) {
	vector<CvPoint2D32f> x1s, x2s;
	x1s.push_back(pc[pos].first);
	x2s.push_back(pc[pos].second);
	IplImage* cos2_index = pcDrawer.create(im1, im2, x1s, x2s);
	IplImage* cos1_index = pcDrawer.create(im1, x1s, x2s);
	showImage("cos2_index", cos2_index);
	showImage("cos1_index", cos1_index);
	cvReleaseImage(&cos2_index);
	cvReleaseImage(&cos1_index);
}

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "i:L:R:q")) != EOF) {
		switch (c) {
			case 'i': input = argv[optind - 1]; break;
			case 'L': leftImage = argv[optind - 1]; break;
			case 'R': rightImage = argv[optind - 1]; break;
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

	if (leftImage == NULL) {
		cout<<"not left image input!"<<endl;
		printf(usage, argv[0]);
		exit(1);
	}
	im1 = cvLoadImage(leftImage);
	if (im1 == NULL) {
		cout<<"fail to load left image: "<<leftImage<<endl;
		exit(1);
	}

	if (rightImage == NULL) {
		cout<<"not right image input!"<<endl;
		printf(usage, argv[0]);
		exit(1);
	}
	im2 = cvLoadImage(rightImage);
	if (im2 == NULL) {
		cout<<"fail to load right image: "<<rightImage<<endl;
		exit(1);
	}

	if (input == NULL) {
		cout<<"not point correspondences file input!"<<endl;
		printf(usage, argv[0]);
		exit(1);
	}

	try {
		fvision::load(input, pc);
	}catch (ifstream::failure e) {
		cout<<"fail to read point correspondences input file:"<<input<<endl;
		printf(usage, argv[0]);
		exit(1);
	}
	if (pc.size() == 0) {
		cout<<"no data read or fail to load point correspondences file!"<<endl;
		exit(1);
	}

	pcDrawer.setLineDrawer(LineDrawer(Colors::RED));
	IplImage* cos1 = pcDrawer.create(im1, pc);
	IplImage* cos2 = pcDrawer.create(im1, im2, pc);

	showImage("cos1", cos1);
	showImage("cos2", cos2);

	IplImage* cos1_index = cvCloneImage(im1);
	showImage("cos1_index", cos1_index);
	IplImage* cos2_index = cvCloneImage(cos2);
	showImage("cos2_index", cos2_index);

	cvCreateTrackbar("index", "cos2_index", &selPointIndex, (int)pc.size() - 1, onSelPointIndexChange);

	cvWaitKey(0);

    return 0;
}
