#ifndef FVISION_AVI_IMAGE_INPUT_HANDLER_H_
#define FVISION_AVI_IMAGE_INPUT_HANDLER_H_

#include "isio.h"

#include <highgui.h>

#include <string>

namespace fvision { namespace impl {

using namespace fvision;

class AviImageSequenceReader : public ImageSequenceReader {
public:
	AviImageSequenceReader(const std::string& filepath);
	~AviImageSequenceReader(void);

public:
	IplImage* read();
	bool reset();

private:
	std::string filepath;
	CvCapture* capture;
	IplImage* frame;
	bool flip;
};

}}
#endif // FVISION_AVI_IMAGE_INPUT_HANDLER_H_