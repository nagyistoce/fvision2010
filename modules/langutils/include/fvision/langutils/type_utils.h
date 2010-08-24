#pragma once

#include <vector>

namespace fvision {

typedef std::vector<int>     v_int;
typedef std::vector<double>  v_double;
typedef std::vector<float>   v_float;

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

}
