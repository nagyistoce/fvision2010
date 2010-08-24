#ifndef PATH_REGEX_IMAGE_OUTPUT_HANDLER_H_
#define PATH_REGEX_IMAGE_OUTPUT_HANDLER_H_

#include "isio.h"

#include <string>

namespace fvision { namespace impl {

using namespace fvision;

//
class PathRegexImageSequenceWriter : public ImageSequenceWriter {
public:
	PathRegexImageSequenceWriter(const std::string& pathRegex, int startIndex, int interval = 1);
	~PathRegexImageSequenceWriter(void);

public:
	bool write(const IplImage* im);
	const char* getErrorMessage() const { return errorMessage.c_str(); }

private:
	std::string pathRegex;
	int startIndex;
	int interval;

	char* pathBuf;

	int curIndex;

	std::string errorPrefix;
	std::string errorMessage;
};

}}
#endif // PATH_REGEX_IMAGE_OUTPUT_HANDLER_H_
