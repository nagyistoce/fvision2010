#ifndef WINDOW_IMAGE_OUTPUT_HANDLER_H_
#define WINDOW_IMAGE_OUTPUT_HANDLER_H_

#include "../ImageSequenceWriter.h"

#include <string>

namespace fvision { namespace impl {

using namespace fvision;

class WindowImageSequenceWriter : public ImageSequenceWriter {
public:
	WindowImageSequenceWriter(const std::string& windowName, int waitTime = 30);
	~WindowImageSequenceWriter(void);

public:
	bool write(const IplImage* im);

private:
	std::string windowName;
	int waitTime;
};

}}
#endif // WINDOW_IMAGE_OUTPUT_HANDLER_H_
