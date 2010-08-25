#include <image_filter.h>

#include "SmartImageFilterImpl.h"
#include "SequentialImageFilterImpl.h"

#include "ResizeImageFilter.h"
#include "SmoothImageFilter.h"
#include "ToGrayImageFilter.h"
#include "BinarizeImageFilter.h"

namespace fvision {

namespace image_filters {

SmartImageFilter* smart(ImageFilter* filter) {
	return new SmartImageFilterImpl(filter);
}

SequentialImageFilter* chain(ImageFilter* filter) {
	return new SequentialImageFilterImpl(filter);
}

SequentialImageFilter* chain(ImageFilter* filter1, ImageFilter* filter2, ImageFilter* filter3) {
	SequentialImageFilter* filterChain = new SequentialImageFilterImpl(filter1);
	if (filter2 != NULL) filterChain->addFilter(filter2);
	if (filter3 != NULL) filterChain->addFilter(filter3);
	return filterChain;
}

ImageFilter* resize(const CvSize& size) {
	return ResizeImageFilter::createWithNewSize(size);
}

ImageFilter* resize(double ratio) {
	return ResizeImageFilter::createWithNewRatio(ratio);
}

ImageFilter* toGray() {
	return new ToGrayImageFilter();
}

ImageFilter* toBinary(double threshold, int cmpOp) {
	return BinarizeImageFilter::create(threshold, cmpOp);
}

ImageFilter* smooth(int smoothType, 
	int param1, 
	int param2, 
	double param3, 
	double param4)
{
	return SmoothImageFilter::create(smoothType, param1, param2, param3, param4);
}

}
}