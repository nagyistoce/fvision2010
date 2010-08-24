#include "AviImageSequenceWriter.h"

namespace fvision { namespace impl {

AviImageSequenceWriter::AviImageSequenceWriter(const std::string& filepath, int fps) {
	this->filepath = filepath;
	this->fps = fps;
	this->pvw = NULL;
}

AviImageSequenceWriter::~AviImageSequenceWriter(void) {
	if (pvw != NULL) 	cvReleaseVideoWriter(&pvw);
}

bool AviImageSequenceWriter::write(const IplImage* im) {
	assert (im != NULL);
	if (im == NULL) return true;
	if (pvw == NULL) {
		pvw = cvCreateVideoWriter(filepath.c_str(), -1, fps, cvGetSize(im));
		if (pvw == NULL) {
			errorMessage = "can not create avi file with path: " + filepath;
			return false;
		}
	}
	cvWriteFrame(pvw, im);
	return true;
}

}}