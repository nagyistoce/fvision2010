#ifndef FVISION_STRING_UTILS_H_
#define FVISION_STRING_UTILS_H_

#include <vector>
#include <string>

namespace fvision {

std::string trim(const std::string& s,const std::string& drop = " ");

// "a = b" => <a, b>
bool decomposePair(const std::string& s, std::string& pre, std::string& post);

// xxxx/ssss/yy.zz => xxxx/ssss yy zz
void decomposePath(const char* path, std::string& dir, std::string& filename, std::string& filetype);

// decompose directory path to a set of directories based on delimeter
std::vector<std::string> decomposeDirectoryPath(const std::string& dirPath);

const char* getFileName(const char* path, std::string& filename);
}
#endif // FVISION_STRING_UTILS_H_