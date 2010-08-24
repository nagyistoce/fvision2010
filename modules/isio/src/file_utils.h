#ifndef FVISION_ISIO_FILE_UTILS_H_
#define FVISION_ISIO_FILE_UTILS_H_

#include <vector>
#include <string>

namespace fvision {

namespace isio_internal {
class JFile {
public:
	bool listFiles(const char* path, std::vector<std::string>& files);

	const std::string& getErrorMessage() const { return errorMessage; }
private:
	std::string errorMessage;
};
}
}
#endif // FVISION_ISIO_FILE_UTILS_H_