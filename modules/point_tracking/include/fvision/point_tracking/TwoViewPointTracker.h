#ifndef FVISION_TWO_VIEW_POINT_TRACKER_H_
#define FVISION_TWO_VIEW_POINT_TRACKER_H_

#include <cv.h>

#include <vector>

namespace fvision {

/**
 * abstract class for two view point tracker, like sift, mops, klt and others
 * input two images, output corresponding points
 */
class TwoViewPointTracker {
public:
	TwoViewPointTracker(void);
	virtual ~TwoViewPointTracker(void);

public:
	virtual void compute( const IplImage* im1, const IplImage* im2, std::vector<CvPoint2D32f>& x1s, std::vector<CvPoint2D32f>& x2s ) = 0;
};

}
#endif // FVISION_TWO_VIEW_POINT_TRACKER_H_