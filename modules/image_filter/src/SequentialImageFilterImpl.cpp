#include "SequentialImageFilterImpl.h"

#include "image_filter_utils.h"

using namespace std;

namespace fvision {

SequentialImageFilterImpl::SequentialImageFilterImpl(ImageFilter* filter) {
	if (filter != NULL) addFilter(filter);
}

SequentialImageFilterImpl::~SequentialImageFilterImpl(void) {
	for (size_t i = 0; i < filters.size(); i++) delete filters[i];
}

SequentialImageFilter* SequentialImageFilterImpl::addFilter(ImageFilter* filter) {
	assert (filter != NULL);
	filters.push_back(ImageFilterFactory().createSmartImageFilter(filter));
	return this;
}

IplImage* SequentialImageFilterImpl::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	if (filters.size() == 0) {
		if (buffer == NULL) {
			buffer = cvCloneImage(im);
		} else {
			assert (filterIsImageHeaderEqual(im, buffer));
			cvCopy(im, buffer);
		}
		return buffer;
	}
	if (filters.size() == 1) {
		return filters[0]->getInternalFilter()->process(im, buffer);
	}
	IplImage* prevOutput = filters[0]->process(im);
	for (size_t i = 1; i < filters.size() - 1; i++) {
		prevOutput = filters[i]->process(prevOutput);
	}
	size_t lastIndex = filters.size() - 1;
	buffer = filters[lastIndex]->getInternalFilter()->process(prevOutput, buffer);
	return buffer;
}

}