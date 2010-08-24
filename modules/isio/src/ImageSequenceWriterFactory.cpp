#include "isio.h"

#include "PathRegexImageSequenceWriter.h"
#include "AviImageSequenceWriter.h"

#include <iostream>

using namespace std;

namespace fvision {

ImageSequenceWriter* ImageSequenceWriterFactory::pathRegex(const char* pathRegex, int startIndex, int interval /*= 1*/) {
	return new impl::PathRegexImageSequenceWriter(pathRegex, startIndex, interval);
}

ImageSequenceWriter* ImageSequenceWriterFactory::avi(const char* aviPath, int fps /*= 25*/) {
	if (fps < 1) { errorMessage = "fps is less than 1"; return NULL; }
	return new impl::AviImageSequenceWriter(aviPath, fps);
}

void isio::run(ImageSequenceReader* reader, ImageSequenceWriter* writer, bool verbose) {
	IplImage* frame = NULL;
	int i = 0;
	while ((frame = reader->read()) !=	NULL) {
		if (!writer->write(frame)) return;
		if (verbose && ((++i) % 10 == 0)) cout<<"processed "<<i<<" frames ......"<<endl;
	}
}

}