#ifndef FVISION_TO_GRAY_IMAGE_FILTER_H_
#define FVISION_TO_GRAY_IMAGE_FILTER_H_

#include "image_filter.h" 

namespace fvision {

//if the input is not gray scale, convert it to grayscale
class ToGrayImageFilter : public ImageFilter {
public:
	ToGrayImageFilter(void);
	~ToGrayImageFilter(void);

public:
	IplImage* process(const IplImage* im, IplImage* buffer = NULL);

};

}
#endif // FVISION_TO_GRAY_IMAGE_FILTER_H_