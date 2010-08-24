#include "isio.h"

#include "PathRegexImageSequenceReader.h"
#include "AviImageSequenceReader.h"
#include "file_utils.h"

#include <iostream>
using namespace std;

namespace fvision {
class RepeatedImageSequenceReader : public ImageSequenceReader {
public:
	RepeatedImageSequenceReader(const std::string& filepath, int count) {
		this->filepath = filepath;
		this->count = count;
		this->frame = NULL;
		this->loaded = false;
		this->curIndex = 0;
	}
	RepeatedImageSequenceReader(const IplImage* src, int count) {
		this->count = count;
		this->frame = cvCloneImage(src);
		this->loaded = true;
		this->curIndex = 0;
	}
	~RepeatedImageSequenceReader(void) {
		if (frame != NULL) cvReleaseImage(&frame);
	}

public:
	IplImage* read() {
		if (!loaded) {
			frame = cvLoadImage(filepath.c_str());
			loaded = true;
		}
		if (count < 0 || curIndex < count) return frame;
		else return NULL;
	}

	bool reset() { return true; }

private:
	std::string filepath;
	int count;
	IplImage* frame;
	bool loaded;

	int curIndex;
};

class InRange {
public:
	static bool isValid(int startIndex, int endIndex, int interval) {
		return (interval > 0) && (startIndex < endIndex);
	}

	static InRange createFromThree(int startIndex, int endIndex, int interval) {
		assert(isValid(startIndex, endIndex, interval));

		return InRange(startIndex, endIndex, interval);
	}

	static InRange createFromTwo(int startIndex, int endIndex) {
		return InRange(startIndex, endIndex, 1);
	}

private:
	InRange(int startIndex, int endIndex, int interval) {
		this->startIndex = startIndex;
		this->endIndex = endIndex;
		this->interval = interval;
	}

public:
	int startIndex;
	int endIndex;
	int interval;
};

// <path>(<range>)
// if <path> contains %, it's a regex path
// 
// <path>#<num>
//
class InputStringStructure {

public:
	ImageSequenceReader* parse(const char* str) {
		using namespace std;
		 
		string sstr = str;
		size_t pos = sstr.find('#');
		if (pos != string::npos) { //this is image + repeat
			string prev = sstr.substr(0, pos);
			string post = sstr.substr(pos + 1);
			return new RepeatedImageSequenceReader(prev, atoi(post.c_str()));
		}

		ImageSequenceReader* reader = NULL;
		ImageSequenceReaderFactory factory;

		pos = sstr.find('%');
		if (pos != string::npos) { //this is path regex, to do
			return NULL;
		}

		string dir, filename, suffix;
		decomposePath(str, dir, filename, suffix);
		//cout<<"suffix:["<<suffix<<"]"<<endl;
		if (suffix.length() == 0) { //dir
			reader = factory.dir(str);
		} else if (strcmp(suffix.c_str(), "avi") == 0) {  //avi
			reader = factory.avi(str);
		} else { //regard as still image
			reader = factory.repeatImage(str, 1);
		}
		errorMessage = factory.getErrorMessage();

		return reader;
	}

	const char* getErrorMessage() { return errorMessage.c_str(); }

private:
	void decomposePath(const char* path, string& dir, string& filename, string& filetype) {
		string spath = string(path);
		int dotpos = (int)spath.rfind('.');
		int slash1 = (int)spath.rfind('/');
		int slash2 = (int)spath.rfind('\\');
		int slashpos = (slash1 >= slash2) ? slash1 : slash2;
		if (dotpos > slashpos) {
			filetype.append(spath.substr(dotpos + 1));
			filename.append(spath.substr(slashpos + 1, dotpos - slashpos - 1));
		}
		if (slashpos > 0) dir.append(spath.substr(0, slashpos));
	}

private:
	std::string errorMessage;
};

ImageSequenceReader* ImageSequenceReaderFactory::pathRegex(const char* pathRegex, int startIndex, int endIndex, int interval /*= 1*/) {
	return new impl::PathRegexImageSequenceReader(pathRegex, startIndex, endIndex, interval);
}

ImageSequenceReader* ImageSequenceReaderFactory::avi(const char* aviPath) {
	return new impl::AviImageSequenceReader(aviPath);
}

ImageSequenceReader* ImageSequenceReaderFactory::repeatImage(const IplImage* src, int times) {
	return new RepeatedImageSequenceReader(src, times);
}

ImageSequenceReader* ImageSequenceReaderFactory::repeatImage(const char* imagePath, int times) {
	IplImage* src = cvLoadImage(imagePath);
	if (src == NULL) {
		errorMessage = std::string("can not load image from ") + imagePath;
		return NULL;
	} else {
		ImageSequenceReader* reader = repeatImage(src, times);
		cvReleaseImage(&src);
		return reader;
	}
}

class FilesImageSequenceReader : public ImageSequenceReader {
public:
	FilesImageSequenceReader(const std::vector<std::string>& filenames) {
		this->filenames = filenames;
		this->frame = NULL;
		this->curIndex = 0;
	}
	~FilesImageSequenceReader(void) {
		if (frame != NULL) cvReleaseImage(&frame);
	}

public:
	IplImage* read() {
		while (curIndex < filenames.size()) {
			if (frame != NULL) cvReleaseImage(&frame);
			frame = cvLoadImage(filenames[curIndex].c_str());
			curIndex++;
			if (frame != NULL) return frame;
		}
		return NULL;
	}

	bool reset() { curIndex = 0; return true; }

private:
	std::vector<std::string> filenames;
	IplImage* frame;

	int curIndex;
};

ImageSequenceReader* ImageSequenceReaderFactory::files(const std::vector<std::string>& filenames) {
	return new FilesImageSequenceReader(filenames);
}

ImageSequenceReader* ImageSequenceReaderFactory::dir(const char* dirPath) {
	using namespace isio_internal;
	JFile jfile;
	vector<string> filenames;
	string path = dirPath;
	path = path + "/*.*";
	bool succuess = jfile.listFiles(path.c_str(), filenames);
	if (!succuess) {
		errorMessage = jfile.getErrorMessage();
		return NULL;
	} else {
		//cout<<"filenames size: "<<filenames.size()<<endl; 
		for (int i = 0; i < filenames.size(); i++) {
			filenames[i] = string(dirPath) + "/" + filenames[i];
		}
		return files(filenames);
	}
}

ImageSequenceReader* ImageSequenceReaderFactory::createFromString(const char* str) {
	InputStringStructure iss;
	ImageSequenceReader* ih = iss.parse(str);
	errorMessage = iss.getErrorMessage();
	return ih;
}

}