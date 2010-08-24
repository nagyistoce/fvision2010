#include "PathRegexImageSequenceWriter.h"

#include <highgui.h>
#include <iostream>
#include <cstdio>

using namespace fvision;
using namespace std;

namespace fvision { namespace impl {

PathRegexImageSequenceWriter::PathRegexImageSequenceWriter(const std::string& pathRegex, int startIndex, int interval /*= 1*/) {
	this->pathRegex = pathRegex;
	this->startIndex = startIndex;
	this->interval = interval;

	curIndex = startIndex;

	pathBuf = new char[pathRegex.size() + 10];

	errorPrefix = "can not write to ";

	cvSetErrMode(CV_ErrModeSilent);

}

PathRegexImageSequenceWriter::~PathRegexImageSequenceWriter(void) {
	delete[] pathBuf;
}

bool PathRegexImageSequenceWriter::write(const IplImage* im) {

    snprintf(pathBuf, pathRegex.size() + 10, pathRegex.c_str(), curIndex);
	cvSaveImage(pathBuf, im);

	curIndex += interval;

	if (curIndex % 10 == 0) cout<<"output "<<curIndex<<"..........."<<endl;

	if (cvGetErrStatus() < 0) {
		errorMessage = errorPrefix + pathBuf;
		return false;
	}
	return true;
}

}}
