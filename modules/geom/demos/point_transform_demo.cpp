#include <gtest/gtest.h>

#include <fvision/geom.h>
#include <fvision/cvutils.h>

#include <iostream>

using namespace std;
using namespace fvision;


class point_transform_test : public testing::Test {
protected:
	virtual void SetUp() {
		H = getIdentity();
		p = cvPoint(10, 20);
		np = cvPoint(0, 0);
	}

	virtual void tearDown() {
		cvReleaseMat(&H);
	}

	CvPoint p;
	CvPoint np;
	CvMat* H;
};

TEST_F(point_transform_test, point2d_transform) {
	cout<<"np: "<<np<<endl;
    Point2DTransformer<CvPoint>* t = point2d_transformers::homography<CvPoint>(H);
	np = t->transform(p);
	delete t;
	cout<<"p: "<<p<<endl;
	cout<<"np: "<<np<<endl;
}
