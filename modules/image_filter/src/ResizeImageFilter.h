#ifndef RESIZE_IMAGE_FILTER_H_
#define RESIZE_IMAGE_FILTER_H_

#include "image_filter.h"

namespace fvision {

class ResizeImageFilter : public ImageFilter {
private:
	ResizeImageFilter(void);
public:
	~ResizeImageFilter(void);
public:
	static ResizeImageFilter* createWithNewSize(const CvSize& size);
	static ResizeImageFilter* createWithNewRatio(double ratio);

public:
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

private:
	void determineNewSize(const CvSize& inputImageSize);

private:
	double ratio;
	CvSize newSize;
	bool useTargetBufferSize;

};

}

#endif // RESIZE_IMAGE_FILTER_H_
