//track points in an image sequence using klt
//skip duplicate frames, since 0.2

static char usage[] = "\n klt point tracking version 0.3 \n\
\n\
usage: %s -i filepath -f ptdb.txt [options] \n\
\n\
reads an image sequence\n\
runs tracking\n\
write result to ptdb.txt\n\
\n\
options:\n\
-d             draw tracking images\n\
-c filepath    write tracking images to filepath\n\
-p param       corner detector params ('400 0.02 8')\n\
-m num         maximum tracking frame num\n\
-s             skip duplicated frames (default is false)\n\
-q             quiet (turn off debugging output)\n\
\
Example:\n\
//read abc.avi, output the tracking results to abc_ptdb, draw intermediate tracking results\n\
klt_point_tracking -i abc.avi -f abc_ptdb.txt -d\
\
output format\n\
num_of_total_features\n\
frameId, pointId, x y\n\
frameId, pointId, x y\n\
...\n\
\n";

#include <cv.h>
#include <highgui.h>

#include <fvision/cvutils.h>
#include <fvision/langutils.h>
#include <fvision/point_tracking.h>
#include <fvision/isio.h>
#include <fvision/cvdraw.h>

#include <fstream>
#include <sstream>

using namespace std;
using namespace fvision;

//-i
char* filepath    = NULL;
//-f
char* outpath      = NULL;
//-q
bool  verbose     = true;
//-d
bool  drawImages  = false;
//-c
char* aviOutput   = NULL;
//-s
bool skipDuplicatedFrames = false;

int      cornerCount        = 400;
double   qualityLevel       = 0.02;
int      pointsMinDistance  = 8;

int      keyFrameInterval   = 3;

//-m
size_t   maxFrameNum        = -1;


PointDrawer pointDrawer("+");
TextDrawer textDrawer;

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "i:f:dc:p:m:sq")) != EOF) {
		switch (c) {
			case 'i': filepath     = argv[optind - 1]; break;
			case 'f': outpath       = argv[optind - 1]; break;
			case 'd': drawImages   = true;             break;
			case 'c': aviOutput    = argv[optind - 1]; break;
			case 'p': {
				istringstream iss(argv[optind - 1]);
				iss>>cornerCount>>qualityLevel>>pointsMinDistance;
				break;
					  }
			case 'm': maxFrameNum = atoi(argv[optind - 1]); break;
			case 's': skipDuplicatedFrames = true; break;
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

void checkInputs() {

	exitIfNull(filepath,  "Please input filepath with '-i <filepath>'!");
	exitIfNull(outpath,    "Please set outpath filepath with '-f <filepath>'!");
}

class FrameChangeChecker {
public:
	FrameChangeChecker(double threshold);
	~FrameChangeChecker(void);

public:
	bool isNewFrameChange(const IplImage* im);
	double getAvgDiff() const { return avgDiff; }

private:
	double computeFrameDifference(const IplImage* im);

private:
	IplImage* prevFrame;
	IplImage* diffBuf;
	double avgDiff;
	double threshold;
};

FrameChangeChecker::FrameChangeChecker(double threshold) {
	prevFrame = NULL;
	this->threshold = threshold;
	avgDiff = 0;
}

FrameChangeChecker::~FrameChangeChecker(void) {
	cvReleaseImage(&prevFrame);
}

bool FrameChangeChecker::isNewFrameChange(const IplImage* im) {
	if (prevFrame == NULL) {
		prevFrame = cvCloneImage(im);
		diffBuf = cvCloneImage(im);
		return true;
	}
	avgDiff = computeFrameDifference(im);
	if (avgDiff > threshold) {
		cvCopy(im, prevFrame);
		return true;
	}
	return false;
}

double FrameChangeChecker::computeFrameDifference(const IplImage* im) {
	cvAbsDiff(im, prevFrame, diffBuf);
	CvScalar avg = cvAvg(diffBuf);
	return avg.val[0];
}

int main(int argc, char* argv[]) {

	processCommandLine(argc, argv);

	checkInputs();

	//=========== Set up PointTracker =============>
	PointTrackerFactory factory;
	PointTracker* tracker = factory.createPyrKLT(cornerCount, qualityLevel, pointsMinDistance, keyFrameInterval);

	//=========== Begin Tracking ==================>
	ImageSequenceReaderFactory isrFactory;
	ImageSequenceReader* pisr = isrFactory.createFromString(filepath);

	IplImage* frame   = NULL;   //image buffer
	IplImage* gframe  = NULL;   //gray image buffer

	int frameId = 0;

	if (drawImages) {
		cvNamedWindow("frame", 1);
	}

	vector<CvPoint2D32f> pts;
	vector<int> ids;

	//CvFont font;
	//cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.3f, 0.3f);
	textDrawer.setColor(CV_RGB(255, 0, 0));

	FrameChangeChecker checker(8);

	ImageSequenceWriter* pWriter = NULL;

	while ((frame = pisr->read()) != NULL && (size_t)frameId < maxFrameNum) {

		if (verbose && frameId % 10 == 0) {
			cout<<"processed "<<frameId<<" frames ..............."<<endl;
		}

		if (gframe == NULL) {
			gframe = cvCreateImage(cvGetSize(frame), 8, 1);
		}

		cvCvtColor(frame, gframe, CV_BGR2GRAY);

		if (skipDuplicatedFrames) {
			bool change = checker.isNewFrameChange(gframe);
			if (change) {
				tracker->addFrame(frameId, gframe);
			} else {
				cout<<"no change "<<frameId<<endl;
			}
		} else {
			tracker->addFrame(frameId, gframe);
		}

		if (drawImages || aviOutput != NULL) {

			tracker->getTrackingResults()->getIndexedObjectsByFrameId(frameId, ids, pts);

			drawPointsWithId(frame, pts, ids, pointDrawer, textDrawer);

			if (drawImages) {
				showImage("frame", frame);
				cvWaitKey(1000);
			}

			if (aviOutput != NULL) {
				if (pWriter == NULL) {
					ImageSequenceWriterFactory writerFactory;
					int FPS = 25;
					pWriter = writerFactory.avi(aviOutput, FPS);
					if(pWriter == NULL) {
						if (verbose) { cout<<"can not create avi with path: "<<aviOutput<<endl;	}
						delete tracker;
						if (gframe != NULL) { cvReleaseImage(&gframe); }
						exit(1);
					}
				}
				pWriter->write(frame);
			}
		}

		frameId++;

	}

	if (pWriter != NULL) {
		delete pWriter;
	}

	save(outpath, tracker->getTrackingResults());

	delete tracker;

	if (gframe != NULL) { cvReleaseImage(&gframe); }

	if (verbose) cout<<"finished .............."<<endl;

	cvWaitKey(0);

    return 0;
}
