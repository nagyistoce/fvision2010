#ifndef FVISION_MEM_TEMPLATE_H_
#define FVISION_MEM_TEMPLATE_H_

namespace fvision {
	
template <typename T>
void deepDelete(const std::vector<T*>& v) {
	for (size_t i = 0; i < v.size(); i++) {
		delete v[i];
	}
}


}
#endif // FVISION_MEM_TEMPLATE_H_