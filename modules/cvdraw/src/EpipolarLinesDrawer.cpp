#include "EpipolarLinesDrawer.h"

#include <fvision/cvutils.h>
#include "LineDrawer.h"
#include "drawing.h"

namespace fvision {

EpipolarLinesDrawer::EpipolarLinesDrawer(void) {
	lineDrawer = new LineDrawer();
}

EpipolarLinesDrawer::~EpipolarLinesDrawer(void) {
	delete lineDrawer;
}

void drawEpipolarLine(IplImage* im1, IplImage* im2, const CvMat* F, CvMat* e1, CvPoint2D32f p1, const LineDrawer& lineDrawer) {
	CvMat* mx1 = hmatFromPoint2D(p1);
	CvMat* l1 = cvCreateMat(3, 1, CV_64FC1);
	cvCrossProduct(e1, mx1, l1);
	CvMat* l2 = matMul(F, mx1);

	drawLine(im1, l1, lineDrawer);
	drawLine(im2, l2, lineDrawer);

	cvReleaseMat(&mx1);
	cvReleaseMat(&l1);
	cvReleaseMat(&l2);
}

void EpipolarLinesDrawer::draw(const CvMat* F, IplImage* leftImage, IplImage* rightImage) const {

	CvMat* e1 = null(F);

	CvSize s = cvGetSize(leftImage);
	for (int i = 1; i < 5; i++) {
		drawEpipolarLine(leftImage, rightImage, F, e1, cvPoint2D32f(0, i * s.height / 5), *lineDrawer);
		drawEpipolarLine(leftImage, rightImage, F, e1, cvPoint2D32f(i * s.width / 5, 0), *lineDrawer);
		drawEpipolarLine(leftImage, rightImage, F, e1, cvPoint2D32f(s.width, i * s.height / 5), *lineDrawer);
		drawEpipolarLine(leftImage, rightImage, F, e1, cvPoint2D32f(i * s.width / 5, s.height), *lineDrawer);
	}

	cvReleaseMat(&e1);	
}

}