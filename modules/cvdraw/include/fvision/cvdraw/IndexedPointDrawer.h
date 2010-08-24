#ifndef FVISION_INDEXED_POINT_DRAWER_H_
#define FVISION_INDEXED_POINT_DRAWER_H_

#include "PointDrawer.h"
#include "TextDrawer.h"

#include <vector>

namespace fvision {

class IndexedPointDrawer {
public:
	IndexedPointDrawer(void);
	IndexedPointDrawer(const PointDrawer& pointDrawer, const TextDrawer& textDrawer, const CvPoint& offset = cvPoint(5, 5));
	~IndexedPointDrawer(void);

public:
	void drawPointAndId(IplImage* im, const CvPoint2D32f& pt, int id);
	void drawPointsAndIds(IplImage* im, const std::vector<CvPoint2D32f>& pts, const std::vector<int>& ids);

public:
	PointDrawer pointDrawer;
	TextDrawer textDrawer;
	CvPoint offset;
	char numberText[20];
};

}
#endif // FVISION_INDEXED_POINT_DRAWER_H_