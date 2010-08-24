#ifndef FVISION_AVI_IMAGE_OUTPUT_HANDLER_H_
#define FVISION_AVI_IMAGE_OUTPUT_HANDLER_H_

#include "isio.h"

#include <string>
#include <highgui.h>

namespace fvision { namespace impl {

using namespace fvision;

class AviImageSequenceWriter : public ImageSequenceWriter {
public:
	AviImageSequenceWriter(const std::string& filepath, int fps = 25);
	~AviImageSequenceWriter(void);

public:
	bool write(const IplImage* im);
	const char* getErrorMessage() const { return errorMessage.c_str(); }

private:
	CvVideoWriter* pvw;
	std::string filepath;
	int fps;
	std::string errorMessage;
};

}}

#endif // FVISION_AVI_IMAGE_OUTPUT_HANDLER_H_