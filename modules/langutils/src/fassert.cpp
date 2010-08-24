#include <langutils/fassert.h>

#include <iostream>
#include <cassert>
#include <cstdlib>

using namespace std;

namespace fvision {

	bool boolAssertNotNull(void* pointer, const char* message) {
		if (pointer == NULL) {
			cerr<<message<<endl;
			return false;
		}
		return true;
	}

	void exitIfNull(void* pointer, const std::string& message) {
		if (pointer == NULL) {
			cerr<<message<<endl;
			exit(1);
		}
	}

	void exitIfFalse(bool v, const char* message) {
		if (!v) {
			cerr<<message<<endl;
			exit(1);
		}
	}

	void exitIf(bool v, const std::string& message) {
		if (v) {
			cerr<<message<<endl;
			exit(1);
		}
	}
}
