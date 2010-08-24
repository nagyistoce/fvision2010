#include "WindowImageSequenceWriter.h"

#include <highgui.h>

using namespace fvision;

namespace fvision { namespace impl {

WindowImageSequenceWriter::WindowImageSequenceWriter(const std::string& windowName, int waitTime) {
	assert (windowName.size() > 0 && windowName.size() < 100 && waitTime >= 0);
	this->windowName = windowName;
	this->waitTime = waitTime;
	cvNamedWindow(windowName.c_str());
}

WindowImageSequenceWriter::~WindowImageSequenceWriter(void) {
	cvDestroyWindow(windowName.c_str());
}

bool WindowImageSequenceWriter::write(const IplImage* im) {
	cvShowImage(windowName.c_str(), im);
	if (waitTime > 0) cvWaitKey(waitTime);
	return true;
}

}}