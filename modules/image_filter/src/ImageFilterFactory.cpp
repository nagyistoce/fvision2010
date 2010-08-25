#include "image_filter.h"

#include "SmartImageFilterImpl.h"
#include "SequentialImageFilterImpl.h"

#include "ResizeImageFilter.h"
#include "SmoothImageFilter.h"
#include "ToGrayImageFilter.h"
#include "BinarizeImageFilter.h"

namespace fvision {

ImageFilterFactory::ImageFilterFactory(void) { }

ImageFilterFactory::~ImageFilterFactory(void) { }


SmartImageFilter* ImageFilterFactory::createSmartImageFilter(ImageFilter* filter) {
	return new SmartImageFilterImpl(filter);
}

SequentialImageFilter* ImageFilterFactory::createSequentialImageFilter(ImageFilter* filter) {
	return new SequentialImageFilterImpl(filter);
}

ImageFilter* ImageFilterFactory::createSmoothImageFilter(
	int smoothType, 
	int param1, 
	int param2, 
	double param3, 
	double param4
) {
	return SmoothImageFilter::create(smoothType, param1, param2, param3, param4);
}

ImageFilter* ImageFilterFactory::createResizeImageFilter(const CvSize& size) {
	return ResizeImageFilter::createWithNewSize(size);
}

ImageFilter* ImageFilterFactory::createResizeImageFilterWithRatio(double ratio) {
	return ResizeImageFilter::createWithNewRatio(ratio);
}

ImageFilter* ImageFilterFactory::createToGrayImageFilter() {
	return new ToGrayImageFilter();
}

ImageFilter* ImageFilterFactory::createBinarizeImageFilter(double threshold, int cmpOp) {
	return BinarizeImageFilter::create(threshold, cmpOp);
}

}