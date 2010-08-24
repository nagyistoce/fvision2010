#include <langutils/file_utils.h>

#pragma warning(disable:4786) //turn off STL complains

#include <langutils/string_utils.h>

#include <windows.h>

#include "stdio.h"

using namespace std;

namespace fvision {

JFile::JFile(void)
{

}

JFile::~JFile(void)
{
}

vector<string> JFile::listFiles(const char* path) {

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH + 1];  // directory specification

    strncpy (DirSpec, path, (strlen(path)+1) * sizeof(char));
	hFind = FindFirstFileA(DirSpec, &FindFileData);

	vector<string> files;
   if (hFind == INVALID_HANDLE_VALUE) 
   {
      //printf ("Invalid file handle. Error is %u\n", GetLastError());
      return files;
   } 
   else 
   {
	   files.push_back(string(FindFileData.cFileName));
      while (FindNextFileA(hFind, &FindFileData) != 0) 
      {
         files.push_back(string(FindFileData.cFileName));
      }
   }
   FindClose(hFind);
   return files;
}

vector<string> JFile::listDirectories(const char* path) {

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH + 1];  // directory specification

    strncpy (DirSpec, path, strlen(path)+1);
    strncat(DirSpec, "\\*", MAX_PATH + 1);
	hFind = FindFirstFileA(DirSpec, &FindFileData);

	bool first = true;
	vector<string> files;
   if (hFind == INVALID_HANDLE_VALUE) 
   {
      printf ("%s, Invalid file handle. Error is %u\n", DirSpec, GetLastError());
   } 
   else 
   {
      while (FindNextFileA(hFind, &FindFileData) != 0) 
      {
		  if (first) { //skip ".."
			  first = false;
			  continue;
		  }
		  if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
              files.push_back(string(FindFileData.cFileName));
		  }
      }
	  FindClose(hFind);
   }
   return files;
}

bool JFile::createDirectory(const char* path) {
	string s = path;
	for (size_t i = 0; i < s.size(); i++) {
		if ('/' == s[i] || '\\' == s[i]) {
			string subPath = s.substr(0, i);
			if (!isFileExists(subPath.c_str())) {
				if (CreateDirectoryA(subPath.c_str(), NULL) != TRUE) {
					errorMessage = string("can not create ") + subPath;
					return false;
				}
			}
		}
	}
	CreateDirectoryA(path, NULL);
	return true;
}

bool JFile::deleteDirectory(const char* path) {
	SHFILEOPSTRUCTA SHDirOp;
	ZeroMemory(&SHDirOp, sizeof(SHFILEOPSTRUCT));
        SHDirOp.hwnd = NULL;
        SHDirOp.wFunc = FO_DELETE;
        SHDirOp.pFrom = path;
        SHDirOp.fFlags = 
          FOF_MULTIDESTFILES|FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION;

        //The Copying Function
	return SHFileOperationA(&SHDirOp) == 0;
}

bool JFile::createFile(const char* path) {
    FILE* f = fopen(path, "w");
	if (f == NULL) return false;
	else {
		fclose(f);
		return true;
	}
}

bool JFile::deleteFile(const char* path) {
	return DeleteFileA(path) == TRUE;
}

bool JFile::isFileExists(const char* path) {
	DWORD       fileAttr;
	fileAttr = GetFileAttributesA(path);
	if (0xFFFFFFFF == fileAttr)
		return false;
	return true;
}

}
