#ifndef FVISION_IMAGE_FILTER_H_
#define FVISION_IMAGE_FILTER_H_


#include <cv.h>

namespace fvision {

class ImageFilterException;

class ImageFilter {
public:
	virtual ~ImageFilter(void) {}
	/// if input NULL, return NULL.
	/// if buffer == NULL, create an image and return it; 
	/// otherwise, write the filtered image in buffer and return the buffer pointer.
	/// @throw ImageFilterException if the input buffer may not match filter's output. 
	virtual IplImage* process(const IplImage* im, IplImage* buffer = NULL) = 0;
};

/// image filter with automatic memory management.
/// the input buffer is ignored, assert buffer == NULL.
/// responsible for deleting the internal filter
class SmartImageFilter : public ImageFilter {
public:
	virtual ~SmartImageFilter() {}
	virtual ImageFilter* getInternalFilter() const = 0;
};

/// the filters input/output may not match
/// if no contained filter, return a copy
/// this is responsible for deleting the input filters
class SequentialImageFilter : public ImageFilter {
public:
	virtual ~SequentialImageFilter() {}
	virtual SequentialImageFilter* addFilter(ImageFilter* filter) = 0;
};

namespace image_filters {

SmartImageFilter* smart(ImageFilter* filter);
SequentialImageFilter* chain(ImageFilter* filter = NULL);
SequentialImageFilter* chain(ImageFilter* filter1, ImageFilter* filter2, ImageFilter* filter3 = NULL);

ImageFilter* resize(const CvSize& size);
ImageFilter* resize(double ratio);
ImageFilter* toGray();
ImageFilter* toBinary(double threshold, int cmpOp = CV_CMP_GT);
ImageFilter* smooth(int smoothType = CV_GAUSSIAN, 
					int param1 = 3, 
					int param2 = 0, 
					double param3 = 0, 
					double param4 = 0);

} //end namespace fvision::image_filters


/// deprecated
/// the only entry point to create class instances
class ImageFilterFactory {
public:
	ImageFilterFactory();
	~ImageFilterFactory();

public:
	SmartImageFilter* createSmartImageFilter(ImageFilter* filter);

	SequentialImageFilter* createSequentialImageFilter(ImageFilter* filter = NULL);

	// =========== filters =================>
	ImageFilter* createSmoothImageFilter(
		int smoothType = CV_GAUSSIAN, 
		int param1 = 3, 
		int param2 = 0, 
		double param3 = 0, 
		double param4 = 0);

	ImageFilter* createResizeImageFilter(const CvSize& size);

	ImageFilter* createResizeImageFilterWithRatio(double ratio);

	ImageFilter* createToGrayImageFilter();

	ImageFilter* createBinarizeImageFilter(double threshold, int cmpOp = CV_CMP_GT);

};

} //end namespace fvision

#endif // FVISION_IMAGE_FILTER_H_
