#ifndef FVISION_TEXT_DRAWER_H_
#define FVISION_TEXT_DRAWER_H_

#include <cv.h>

namespace fvision {

class TextDrawer {
public:
	TextDrawer();
	TextDrawer(const CvFont& font, const CvScalar& color);
	~TextDrawer(void);

public:
	void draw(IplImage* im, const CvPoint& position, const char* text) const ;
	void draw(IplImage* im, const CvPoint2D32f& position, const char* text) const ;

	void setColor(const CvScalar& color);

private:
	CvFont font;
	CvScalar color;
};

}
#endif // FVISION_TEXT_DRAWER_H_