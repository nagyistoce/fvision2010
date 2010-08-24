//view point tracking results
//input originalPtdb file, image sequence file/directory
//show image window, show feature points with id number
//a slide bar to pick the frame to show
//when the user clicked one point, show the corresponding point's path

static char usage[] = 
"\nptdb_view version 0.3 \n\
\n\
usage: %s -i <images_path> -p <originalPtdb.txt> [options] \n\
\n\
reads image seuqnces and point tracking results file\n\
runs ptdb_view\n\
shows images with points by frame\n\
to view a single point's path, just click the point on the frame\n\
\n\
options:\n\
-m num         max frame number (default is 200)\n\
-r <rptdb.txt> reprojection points file\n\
-t threshold   threshold for inliers and outliers\n\
-q             quiet (turn off debugging output)\n\n";

#include "DoubleIndexer.h"

#include <fvision/langutils.h>
#include <fvision/cvdraw.h>
#include <fvision/memdb.h>
#include <fvision/isio.h>
#include <fvision/geom.h>

#include <iostream>
#include <fstream>

using namespace fvision;
using namespace std;

//-p
char* pointsPath = NULL;
//-i
char* imagesPath = NULL;
//-m
size_t maxFrameNum = 200;
//-q
bool verbose = true;
//-r
char* reprojectionPointsPath = NULL;
//-t
double threshold = 3.0;

typedef MultipleFrameIndexedObjectsDatabase<CvPoint2D32f> CvPoint2D32fMdb;
typedef MultipleFrameIndexedObjectsDatabaseMapImpl<CvPoint2D32f> CvPoint2D32fMdbImpl;


CvPoint2D32fMdb* originalPtdb = NULL;
CvPoint2D32fMdb* reprojectionPtdb = NULL;

vector<int> allFrameIds;
DoubleIndexer frameIdIndexer; //i -> frameId


TextDrawer td(cvFont(0.5), CV_RGB(0, 255, 0));
PointDrawer originalImagePointDrawer("+", CV_RGB(0, 255, 0), 4);
PointDrawer inlierReprojectionImagePointDrawer("+", CV_RGB(0, 255, 0), 4);
PointDrawer outlierReprojectionImagePointDrawer("+", CV_RGB(255, 0, 0), 4);

enum PointWindowStatus {
	ONLY_ORIGINAL_POINT,
	ONLY_REPROJECTION,
	BOTH
};

PointWindowStatus pointWindowStatus = BOTH;

vector<IplImage*> images;

const char* winname = "tracking results";
IplImage* mainWindowImageBuffer = NULL;

int selFrameIndex = 0;


const char* pickedPointTrackWindowName = "picked point track";
const char* pickedPointWindowName = "picked point";
IplImage* pickedPointTrackWindowImageBuffer = NULL;
IplImage* pickedPointWindowImageBuffer = NULL;

int pickedPointId = 0;
vector<int> pickedPointFrameIds;
vector<CvPoint2D32f> pickedPointPositions;

vector<int> pickedPointFrameNewIds;
vector<IplImage*> pickedPointFrames; //

vector<int> pickedPointReprojectionStatus; //1 for inliers, -1 for outliers, 0 for no points
vector<CvPoint2D32f> pickedPointReprojections; //if status is 0, the the point is (0, 0)
vector<double> pickedPointReprojectionError; //if status is 0, the value is -1;

int selPickedPointFrameIndex = 0;

void onSelFrameIndexChange(int pos) {
	cvCopy(images[pos], mainWindowImageBuffer);
	vector<CvPoint2D32f> pts;
	vector<int> ids;
	int frameId = frameIdIndexer.getOriginalId(pos);
	originalPtdb->getIndexedObjectsByFrameId(frameId, ids, pts);
	drawPointsWithId(mainWindowImageBuffer, pts, ids, originalImagePointDrawer, td);
	if (reprojectionPtdb != NULL) {
		reprojectionPtdb->getIndexedObjectsByFrameId(frameId, ids, pts);
		for (size_t i = 0; i < pts.size(); i++) {
			CvPoint2D32f pt = pts[i];
			int pid = ids[i];
			CvPoint2D32f opt = originalPtdb->getObjectByFrameIdAndObjectId(frameId, pid);
			double dist = pointDistance(pt, opt);
			if (dist <= threshold) {
				inlierReprojectionImagePointDrawer.draw(mainWindowImageBuffer, pt);
			} else {
				outlierReprojectionImagePointDrawer.draw(mainWindowImageBuffer, pt);
			}
		}
	}

	cvShowImage(winname, mainWindowImageBuffer);
}

void onSelPickedPointFrameIndexChange(int pos) {
	cvCopy(pickedPointFrames[pos], pickedPointWindowImageBuffer);
	if (pointWindowStatus == BOTH || pointWindowStatus == ONLY_ORIGINAL_POINT) {
		drawPointWithId(pickedPointWindowImageBuffer, pickedPointPositions[pos], pickedPointFrameIds[pos], originalImagePointDrawer, td);
	}
	if (reprojectionPtdb != NULL && pointWindowStatus != ONLY_ORIGINAL_POINT) {
		int status = pickedPointReprojectionStatus[pos];
		if (status == 1) {
			inlierReprojectionImagePointDrawer.draw(pickedPointWindowImageBuffer, pickedPointReprojections[pos]);
		} else if (status == -1) {
			outlierReprojectionImagePointDrawer.draw(pickedPointWindowImageBuffer, pickedPointReprojections[pos]);
		}
	}
	cvShowImage(pickedPointWindowName, pickedPointWindowImageBuffer);
}

int getPointId(int x, int y, int frameId) {	
	vector<CvPoint2D32f> pts;
	vector<int> ids;
	originalPtdb->getIndexedObjectsByFrameId(frameId, ids, pts);
	vector<double> distances(pts.size());
	CvPoint2D32f p = cvPoint2D32f(x, y);
	for (size_t i = 0; i < pts.size(); i++) {
		distances[i] = pointDistance(p, pts[i]);
	}
	int index = min_element(distances.begin(), distances.end()) - distances.begin();
	CvPoint2D32f closestPoint = pts[index];
	double dist = pointDistance(p, closestPoint);
	const int MIN_DISTANCE = 5; //pixels
	if (dist > MIN_DISTANCE) {
		return -1;
	} else return ids[index];
}

void initPickedPointData(int pointId) {

	pickedPointId = pointId;

	originalPtdb->getIndexedObjectsByObjectId(pointId, pickedPointFrameIds, pickedPointPositions);

	int size = pickedPointFrameIds.size();
	pickedPointFrames.resize(size);
	for (size_t i = 0; i < pickedPointFrameIds.size(); i++) {
		int frameId = pickedPointFrameIds[i];
		int newId = frameIdIndexer.getNewId(frameId);
		pickedPointFrames[i] = images[newId];
	}

	if (reprojectionPtdb != NULL) {
		pickedPointReprojectionStatus.resize(size);
		pickedPointReprojections.resize(size);
		pickedPointReprojectionError.resize(size);
		for (size_t i = 0; i < pickedPointFrameIds.size(); i++) {
			int frameId = pickedPointFrameIds[i];
			if (reprojectionPtdb->itemExists(frameId, pointId)) {
				CvPoint2D32f pt = reprojectionPtdb->getObjectByFrameIdAndObjectId(frameId, pointId);
				pickedPointReprojections[i] = pt;
				double dist = pointDistance(pt, pickedPointPositions[i]);
				pickedPointReprojectionError[i] = dist;
				if (verbose) {
					cout<<"frameId: "<<frameId<<" -- "<<dist<<endl;
				}
				if (dist <= threshold) {
					pickedPointReprojectionStatus[i] = 1;
				} else {
					pickedPointReprojectionStatus[i] = -1;
				}
			} else {
				pickedPointReprojectionStatus[i] = 0;
				pickedPointReprojections[i] = cvPoint2D32f(0, 0);
				pickedPointReprojectionError[i] = -1;
			}
		}
	}
}

void updatePickedPointTrackWindow() {
	cvCopy(images[frameIdIndexer.getNewId(pickedPointFrameIds[0])], pickedPointTrackWindowImageBuffer);
	drawPointsWithId(pickedPointTrackWindowImageBuffer, pickedPointPositions, pickedPointFrameIds, originalImagePointDrawer, td);
	for (size_t i = 0; i < pickedPointReprojectionStatus.size(); i++) {
		int status = pickedPointReprojectionStatus[i];
		if (status == 1) {
			inlierReprojectionImagePointDrawer.draw(pickedPointTrackWindowImageBuffer, pickedPointReprojections[i]);
		} else if (status == -1) {
			outlierReprojectionImagePointDrawer.draw(pickedPointTrackWindowImageBuffer, pickedPointReprojections[i]);
		}
	}
	showImage(pickedPointTrackWindowName, pickedPointTrackWindowImageBuffer);	
}

void pointWindowClick(int event, int x, int y, int flags, void* param) {
	if (event == CV_EVENT_LBUTTONUP) {
		switch (pointWindowStatus)
		{
		case BOTH: pointWindowStatus = ONLY_ORIGINAL_POINT; break;
		case ONLY_ORIGINAL_POINT: pointWindowStatus = ONLY_REPROJECTION; break;
		case ONLY_REPROJECTION: pointWindowStatus = BOTH; break;
		default: pointWindowStatus = BOTH;
		}
		onSelPickedPointFrameIndexChange(selPickedPointFrameIndex);
	}
}

void updatePickedPointWindow() {
	cvNamedWindow(pickedPointWindowName);
	selPickedPointFrameIndex = 0;
	cvCreateTrackbar("frame_index", pickedPointWindowName, &selPickedPointFrameIndex, (int)(pickedPointFrames.size() - 1), onSelPickedPointFrameIndexChange);
	cvSetMouseCallback(pickedPointWindowName, pointWindowClick, NULL);
	onSelPickedPointFrameIndexChange(0);
}

void showPointPath(int id) {
	initPickedPointData(id);
	updatePickedPointTrackWindow();
	updatePickedPointWindow();
}

//compute the id of the point which is closest to mouse in frame
void pointPicking(int event, int x, int y, int flags, void* param) {
	if (event == CV_EVENT_LBUTTONUP) {
		int id = getPointId(x, y, allFrameIds[selFrameIndex]);
		cout<<"mouse double clicked: "<<id<<" is picked."<<endl;
		if (id >= 0) showPointPath(id);
	}
}

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "i:p:m:r:t:q")) != EOF) {
		switch (c) {
			case 'i': imagesPath = argv[optind - 1]; break;
			case 'p': pointsPath = argv[optind - 1]; break;
			case 'm': maxFrameNum = atoi(argv[optind - 1]); break;
			case 'r': reprojectionPointsPath = argv[optind - 1]; break;
			case 't': threshold = atof(argv[optind - 1]); break;
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

void readData();
void readAllImages();
//void drawPointsAndIdsImage();
//void trimPtdb(CvPoint2D32fMdb* originalPtdb);
void createMainWindow();

void setParameters() {
	//pointsPath = "H:\\zj\\google code\\fvision_modules\\rec\\_make\\temp\\data\\observation2d\\original_Jan_12_points.txt";
	//imagesPath = "H:\\zj\\data\\robot\\Jan_12";
	pointsPath = "H:\\zj\\experiments\\sfm\\curtis\\0021_0040\\tracking_results.txt";
	imagesPath = "H:/zj/data/medical/Endoscopic videos for Dr. Li/trim/curtis/ei";
	reprojectionPointsPath = "H:\\zj\\experiments\\sfm\\curtis\\0021_0040\\reprojection_image_points.txt";
}

void exitsWithHelp(char* argv[]) {
	printf(usage, argv[0]);
	exit(1);
}

void checkInputs(char* argv[]) {
	if (imagesPath == NULL) {
		cout<<"not image sequence path input! please input image path with -i <path>!"<<endl;
		exitsWithHelp(argv);
	}
	if (pointsPath == NULL) {
		cout<<"not points path input! please input points path with -p <path>!"<<endl;
		exitsWithHelp(argv);
	}
}

int main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();

	checkInputs(argv);

	readData();

	readAllImages();

	createMainWindow();

	cvWaitKey(0);

	delete originalPtdb;

    return 0;
}

void createMainWindow() {
	cvNamedWindow(winname);
	cvShowImage(winname, mainWindowImageBuffer);
	cvSetMouseCallback(winname, pointPicking, NULL);
	cvCreateTrackbar("frame_index", winname, &selFrameIndex, (int)(images.size() - 1), onSelFrameIndexChange);
	onSelFrameIndexChange(0);
}

CvPoint2D32fMdb* readPtdb(const char* path) {
	CvPoint2D32fMdb* ptdb = new CvPoint2D32fMdbImpl();
	load(path, ptdb);
	if (ptdb->size() == 0) {
		cout<<"no points"<<endl;
		exit(1);
	}
	return ptdb;
}

void trimOriginalPtdb(CvPoint2D32fMdb* ptdb, const vector<int>& allFrameIds) {
	vector<int> originalPtdbFrameIds = ptdb->getAllFrameIdVector();
	vector<int> diff = subtraction(originalPtdbFrameIds, allFrameIds);
	for (size_t i = 0; i < diff.size(); i++) {
		ptdb->deleteFrame(diff[i]);
	}
}

void readData() {
	originalPtdb = readPtdb(pointsPath);
	if (reprojectionPointsPath != NULL) {
		reprojectionPtdb = readPtdb(reprojectionPointsPath);
		allFrameIds = reprojectionPtdb->getAllFrameIdVector();
		trimOriginalPtdb(originalPtdb, allFrameIds);
		originalImagePointDrawer.pattern = "o";
	} else {
		allFrameIds = originalPtdb->getAllFrameIdVector();
	}
	//newIdIndexer.initialize(allFrameIds.begin(), allFrameIds.end());
}

void readAllImages() {
	ImageSequenceReader* pisr = ImageSequenceReaderFactory().createFromString(imagesPath);
	IplImage* frame = NULL;
	size_t i = 0;
	size_t maxFrameId = (size_t)getMax(allFrameIds); //sorted
	int frameIdIndex = 0;
	while ((frame = pisr->read()) != NULL && i <= maxFrameId) {
		if (i == allFrameIds[frameIdIndex]) {
			images.push_back(cvCloneImage(frame));
			frameIdIndexer.addId(i, frameIdIndex);
			frameIdIndex++;
		}
		i++;
	}
	if (frameIdIndex != allFrameIds.size()) {
		cout<<"can not load all frames!"<<endl;
		exit(1);
	}
	cout<<"load "<<frameIdIndex<<" frames."<<endl;
	mainWindowImageBuffer = cvCloneImage(images[0]);
	pickedPointTrackWindowImageBuffer = cvCloneImage(images[0]);
	pickedPointWindowImageBuffer = cvCloneImage(images[0]);
	delete pisr;
}
