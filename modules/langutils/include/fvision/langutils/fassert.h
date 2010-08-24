#ifndef FVISION_FASSERT_H_
#define FVISION_FASSERT_H_

#include <string>

/// assert something with message. If not true, print out message.

namespace fvision {

	/// if not true, print message and return false, otherwise return true
	bool boolAssertNotNull(void* pointer, const char* message);

	/// if not true, print message and exit
	void exitIfNull(void* pointer, const std::string& message);

	/// if not true, print message and exit
	void exitIfFalse(bool v, const char* message);

	/// if true, print message and exit
	void exitIf(bool v, const std::string& message);
}
#endif // FVISION_FASSERT_H_