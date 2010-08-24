#ifndef FVISION_FORMAT_TEMPLATE_H_
#define FVISION_FORMAT_TEMPLATE_H_

namespace fvision {

class FormatHelper {
public:
	const char* getNumberString(int id);
	const char* getNumberString(int id, const char* format);

private:
	char idString[20];
};
}
#endif // FVISION_FORMAT_TEMPLATE_H_