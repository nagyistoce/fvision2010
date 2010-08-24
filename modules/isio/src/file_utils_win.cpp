#include "file_utils.h"

#include <windows.h>

#include "stdio.h"

using namespace std;

namespace fvision { namespace isio_internal {

bool JFile::listFiles(const char* path, vector<string>& files) {

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH + 1];  // directory specification

    strncpy (DirSpec, path, (strlen(path)+1) * sizeof(char));
	hFind = FindFirstFileA(DirSpec, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		char str[200];
		sprintf(str, "Invalid file handle. Error is %u\n", GetLastError());
		errorMessage = str;
		return false;
	} else {
		files.clear();
		files.push_back(string(FindFileData.cFileName));
		while (FindNextFileA(hFind, &FindFileData) != 0) {
			files.push_back(string(FindFileData.cFileName));
		}
	}
	FindClose(hFind);
	return true;
}}

}
