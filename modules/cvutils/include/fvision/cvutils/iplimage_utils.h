#ifndef FVISION_IPLIMAGE_UTILS_H_
#define FVISION_IPLIMAGE_UTILS_H_

#include <cv.h>
#include <vector>

namespace fvision {

	/**
	* 		    im1->width     ==  im2->width
	*		 && im1->height    ==  im2->height
	*		 && im1->nChannels ==  im2->nChannels
	*		 && im1->depth     ==  im2->depth
	*
	*/
	bool isImageHeaderEqual(const IplImage* im1, const IplImage* im2);

	bool isPointInsideImage(const IplImage* im, int x, int y);

	IplImage* resize(const IplImage* im, const CvSize& newSize, int interpolation = 1);

	//newSize = oldSize * resizeRatio
	IplImage* resize(const IplImage* im, double resizeRatio, int interpolation = 1);

	void releaseImageVector(std::vector<IplImage*>& images);

}

#endif // FVISION_IPLIMAGE_UTILS_H_
