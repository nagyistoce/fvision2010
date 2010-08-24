#include "iplimage_utils.h"

#include "point_arith.h"

namespace fvision {

bool isImageHeaderEqual(const IplImage* im1, const IplImage* im2) {
	if (im1 == NULL || im2 == NULL) return false;
	return (im1->width     ==  im2->width
		&& im1->height    ==  im2->height
		&& im1->nChannels ==  im2->nChannels
		&& im1->depth     ==  im2->depth);
}


bool isPointInsideImage(const IplImage* im, int x, int y) {
	assert (im != NULL);
	return (x >= 0 && x < im->width && y >= 0 && y < im->height);
}

IplImage* resize(const IplImage* im, const CvSize& newSize, int interpolation) {
	IplImage* nim = cvCreateImage(newSize, im->depth, im->nChannels);
	cvResize(im, nim, interpolation);
	return nim;
}

IplImage* resize(const IplImage* im, double resizeRatio, int interpolation /*= 1*/) {
	return resize(im, cvGetSize(im) * resizeRatio, interpolation);
}

void releaseImageVector(std::vector<IplImage*>& images) {
	for (size_t i = 0; i < images.size(); i++) {
		cvReleaseImage(&images[i]);
		images[i] = NULL;
	}
}


}