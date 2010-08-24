#include "DisparitiesDrawer.h"

#include "drawing.h"
#include "PointDrawer.h"
#include "LineDrawer.h"

using namespace std;

namespace fvision {

DisparitiesDrawer::DisparitiesDrawer(void) {
	pd = new PointDrawer("o");
	ld = new LineDrawer(CV_RGB(255, 0, 0));
}

DisparitiesDrawer::~DisparitiesDrawer(void) {
	delete pd;
	delete ld;
}

}