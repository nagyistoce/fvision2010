#include "PathRegexImageSequenceReader.h"

#include <highgui.h>
#include <cstdio>

namespace fvision { namespace impl {

PathRegexImageSequenceReader::PathRegexImageSequenceReader(const std::string& pathRegex, int startIndex, int endIndex, int interval) {
	this->pathRegex = pathRegex;
	this->startIndex = startIndex;
	this->endIndex = endIndex;
	this->interval = interval;

	curIndex = startIndex;

	pathBuf = new char[pathRegex.size() + 10];

	lastImagePointer = NULL;
}

PathRegexImageSequenceReader::~PathRegexImageSequenceReader(void) {
	delete[] pathBuf;
	if (lastImagePointer != NULL) {
		cvReleaseImage(&lastImagePointer);
	}
}

IplImage* PathRegexImageSequenceReader::read() {
	if (lastImagePointer != NULL) {
		cvReleaseImage(&lastImagePointer);
		lastImagePointer = NULL;
	}
	if (curIndex >= endIndex) return NULL;
    snprintf(pathBuf, pathRegex.size() + 10, pathRegex.c_str(), curIndex);
	lastImagePointer = cvLoadImage(pathBuf, 1);
	curIndex += interval;
	return lastImagePointer;
}

bool PathRegexImageSequenceReader::reset() {
	curIndex = startIndex;
	return true;
}
}}
