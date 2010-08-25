#ifndef FVISION_SMART_IMAGE_FILTER_IMPL_H_
#define FVISION_SMART_IMAGE_FILTER_IMPL_H_

#include "image_filter.h"

#include "ImageTypeInfo.h"

namespace fvision {

class SmartImageFilterImpl : public SmartImageFilter {
public:
	//responsible for deleting filter
	SmartImageFilterImpl(ImageFilter* filter);
	~SmartImageFilterImpl(void);

public:
	//assert buffer == NULL
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

	ImageFilter* getInternalFilter() const { return filter; }

private:
	ImageFilter* filter;
	IplImage* outputBuffer;

	ImageTypeInfo lastInputTypeInfo;
};

}
#endif // FVISION_SMART_IMAGE_FILTER_IMPL_H_
