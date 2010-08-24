#ifndef FVISION_ISIO_H_
#define FVISION_ISIO_H_

#include <cv.h>
#include <highgui.h>
#include <string>
#include <vector>

namespace fvision {

class ImageSequenceReader {
public:
	virtual ~ImageSequenceReader(void) {}
	//the client should not delete the returned image pointer
	//return NULL if no next
	virtual IplImage* read() = 0;

	//back to the beginning
	//return false if the derived class do not support the operation
	virtual bool reset() = 0;
};

class ImageSequenceWriter {
public:
	virtual ~ImageSequenceWriter(void) {}
	//return false if writing failed; use getErrorMessage() to get error information
	virtual bool write(const IplImage* im) = 0;
	virtual const char* getErrorMessage() const { return ""; }
};

namespace isio_internal {

/// the type of *iter should be a char* or a string
template <typename InputIterator>
class IteratorBasedImageSequenceReader : public ImageSequenceReader {
public:
	IteratorBasedImageSequenceReader<InputIterator>(InputIterator first, InputIterator last) {
		this->first = first; this->last = last; this->frame = NULL; this->current = first;
	}
    ~IteratorBasedImageSequenceReader<InputIterator>() {
        if (frame != NULL) cvReleaseImage(&frame);
    }

	//the client should not delete the returned image pointer
	//return NULL if no next
	IplImage* read() {
		if (current == last) return NULL;
		if (frame != NULL) cvReleaseImage(&frame);
        const char* firstImageFile = (std::string(*first)).c_str();
        frame = cvLoadImage(firstImageFile);
		first++;
		return frame;
	}

	//back to the beginning
	//return false if the derived class do not support the operation
	bool reset() {
		current = first;
		return true;
	}

private:
	InputIterator first;
	InputIterator last;
	InputIterator current;
	IplImage* frame;
};
}

//return NULL if creating failed, user can use getErrorMessage() to check the reason
class ImageSequenceReaderFactory {
public:
	ImageSequenceReader* pathRegex(const char* pathRegex, int startIndex, int endIndex, int interval = 1);
	ImageSequenceReader* avi(const char* aviPath);
	ImageSequenceReader* dir(const char* dirPath);
	/// the type of *iter should be a char* or a string
	template <typename InputIterator>
	ImageSequenceReader* iterate(InputIterator first, InputIterator last) {
		return isio_internal::IteratorBasedImageSequenceReader<InputIterator>(first, last);		
	}

	ImageSequenceReader* files(const std::vector<std::string>& filenames);

	/// iteratively return src up to <times>
	/// do not responsible for deleting src
	ImageSequenceReader* repeatImage(const IplImage* src, int times);
	ImageSequenceReader* repeatImage(const char* imagePath, int times);

	//currently only support avi
	ImageSequenceReader* createFromString(const char* str);

	const char* getErrorMessage() const { return errorMessage.c_str(); }

private:
	std::string errorMessage;
};

//return NULL if creating failed, user can use getErrorMessage() to check the reason
class ImageSequenceWriterFactory {
public:
	ImageSequenceWriter* pathRegex(const char* pathRegex, int startIndex, int interval = 1);
	ImageSequenceWriter* avi(const char* aviPath, int fps = 25);

	const char* getErrorMessage() const { return errorMessage.c_str(); }

private:
	std::string errorMessage;
};

namespace isio {
void run(ImageSequenceReader* reader, ImageSequenceWriter* writer, bool verbose = false);
}
}

#endif // FVISION_ISIO_H_
