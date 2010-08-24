#ifndef FVISION_POINT_CORRESPONDENCES_DRAWER_H_
#define FVISION_POINT_CORRESPONDENCES_DRAWER_H_

#include <cv.h>
#include <vector>
#include <utility>

namespace fvision {

class PointDrawer;
class LineDrawer;

class PointCorrespondencesDrawer {
public:
	PointCorrespondencesDrawer(void);
	~PointCorrespondencesDrawer(void);

public:
    typedef std::vector<std::pair<CvPoint2D32f, CvPoint2D32f> > PointPairs;

	IplImage* create(const IplImage* leftImage, const IplImage* rightImage, const PointPairs& pps) const;
	IplImage* create(const IplImage* leftImage, const PointPairs& pps) const;

	IplImage* create(const IplImage* leftImage, const IplImage* rightImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const;
	IplImage* create(const IplImage* leftImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const;

	void draw(IplImage* leftImage, const PointPairs& pps) const;
	void draw(IplImage* leftImage, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) const;
	void draw(IplImage* leftImage, const CvPoint2D32f& x1, const CvPoint2D32f& x2) const;

	void setPointDrawer(const PointDrawer& pd);
	void setLineDrawer(const LineDrawer& ld);

private:
	PointDrawer* pointDrawer;
	LineDrawer* lineDrawer;
};

}
#endif // FVISION_POINT_CORRESPONDENCES_DRAWER_H_
