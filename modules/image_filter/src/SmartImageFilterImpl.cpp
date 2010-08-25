#include "SmartImageFilterImpl.h"

namespace fvision {

SmartImageFilterImpl::SmartImageFilterImpl(ImageFilter* filter) {
	assert (filter != NULL);
	this->filter = filter;
	outputBuffer = NULL;
}

SmartImageFilterImpl::~SmartImageFilterImpl(void) {
	delete filter;
	if (outputBuffer != NULL) cvReleaseImage(&outputBuffer);
}

// if the input type do not change, then buffer do not need change
// otherwise, change buffer
IplImage* SmartImageFilterImpl::process(const IplImage* im, IplImage* buffer /*= NULL*/) {
	if (im == NULL) return NULL;
	ImageTypeInfo newTypeInfo = ImageTypeInfo::getTypeInfo(im);
	if (!(lastInputTypeInfo == newTypeInfo)) {
		if (outputBuffer != NULL) cvReleaseImage(&outputBuffer);
		outputBuffer = NULL;
		lastInputTypeInfo = newTypeInfo;
	}
	outputBuffer = filter->process(im, outputBuffer);
	return outputBuffer;
}

}