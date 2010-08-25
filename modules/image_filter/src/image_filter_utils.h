#ifndef FVISION_IMAGE_FILTER_UTILS_H_
#define FVISION_IMAGE_FILTER_UTILS_H_

#include <cv.h>

namespace fvision {

bool filterIsImageHeaderEqual(const IplImage* im1, const IplImage* im2);

}
#endif // FVISION_IMAGE_FILTER_UTILS_H_