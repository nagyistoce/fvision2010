#ifndef FVISION_SEQUENCE_IMAGE_FILTER_IMPL_H_
#define FVISION_SEQUENCE_IMAGE_FILTER_IMPL_H_

#include "image_filter.h"
#include <vector>

namespace fvision {

class SequentialImageFilterImpl : public SequentialImageFilter {
public:
	SequentialImageFilterImpl(ImageFilter* filter);
	~SequentialImageFilterImpl(void);

public:
	//if no contained filter, return a copy
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

	//this is reponsible for deleting the input filter
	SequentialImageFilter* addFilter(ImageFilter* filter);

private:
	std::vector<SmartImageFilter*> filters;
};

}
#endif // FVISION_SEQUENCE_IMAGE_FILTER_IMPL_H_
