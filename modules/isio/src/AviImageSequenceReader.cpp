#include "AviImageSequenceReader.h"

namespace fvision { namespace impl {

AviImageSequenceReader::AviImageSequenceReader(const std::string& filepath) {
	this->filepath = filepath;
	capture = cvCaptureFromAVI(filepath.c_str());
	frame = NULL;
	flip = false;
}

AviImageSequenceReader::~AviImageSequenceReader(void) {
	if (capture != NULL) cvReleaseCapture(&capture);
}

IplImage* AviImageSequenceReader::read() {
	if (capture == NULL) return NULL;
	frame = cvQueryFrame(capture);
	if (frame != NULL) {
		if (frame->origin == 1) {
			cvFlip(frame, NULL);
			frame->origin = 0;
		}
	}
	return frame;
}

bool AviImageSequenceReader::reset() {
	if (capture != NULL) {
		cvReleaseCapture(&capture);
		capture = cvCaptureFromAVI(filepath.c_str());
		if (capture != NULL) return true;
		else return false;
	}
	return false;
}
}}