#include <langutils/format_utils.h>

#include <cstdio>

namespace fvision {

const char* FormatHelper::getNumberString(int id) {
    snprintf(idString, 20, "%04d", id);
	return idString;
}

const char* FormatHelper::getNumberString(int id, const char* format) {
    snprintf(idString, 20, format, id);
	return idString;
}
}
