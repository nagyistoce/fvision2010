#ifndef PATH_REGEX_IMAGE_INPUT_HANDLER_H_
#define PATH_REGEX_IMAGE_INPUT_HANDLER_H_

#include "isio.h"

#include <string>

namespace fvision { namespace impl {

using namespace fvision;
//
class PathRegexImageSequenceReader : public ImageSequenceReader {
public:
	PathRegexImageSequenceReader(const std::string& pathRegex, int startIndex, int endIndex, int interval = 1);
	~PathRegexImageSequenceReader(void);
public:
	IplImage* read();
	bool reset();

private:
	std::string pathRegex;
	int startIndex;
	int endIndex;
	int interval;

	char* pathBuf;

	int curIndex;

	IplImage* lastImagePointer;
};

}}
#endif // PATH_REGEX_IMAGE_INPUT_HANDLER_H_
