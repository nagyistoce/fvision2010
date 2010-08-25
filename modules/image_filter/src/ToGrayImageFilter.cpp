#include "ToGrayImageFilter.h"

namespace fvision {

ToGrayImageFilter::ToGrayImageFilter(void) {
}

ToGrayImageFilter::~ToGrayImageFilter(void)
{
}

IplImage* ToGrayImageFilter::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	if (buffer == NULL) buffer = cvCreateImage(cvGetSize(im), im->depth, 1);
	else assert (im->width == buffer->width && im->height == buffer->height && im->depth == buffer->depth);
	if (im->nChannels == 1) cvCopy(im, buffer);
	else cvCvtColor(im, buffer, CV_BGR2GRAY);
	return buffer;	
}

}