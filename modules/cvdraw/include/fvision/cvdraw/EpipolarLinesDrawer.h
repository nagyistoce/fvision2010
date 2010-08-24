#pragma once

#include <cv.h>

namespace fvision {

class LineDrawer;

class EpipolarLinesDrawer {
public:
	EpipolarLinesDrawer(void);
	~EpipolarLinesDrawer(void);

public:
	void draw(const CvMat* F, IplImage* leftImage, IplImage* rightImage) const ;

private:
	LineDrawer* lineDrawer;
};

}