#include <fvision/image_filter.h>

#include <highgui.h>

#include <iostream>

using namespace std;
using namespace fvision;

int main(int argc, char* argv[]) {

	IplImage* src = cvLoadImage(argv[1]);

	if (src == NULL) { cout<<"fail to load image: ["<<argv[1]<<"]"<<endl; exit(1); }

	ImageFilter* smoothFilter = image_filters::smooth();

	IplImage* dst = smoothFilter->process(src, NULL);

	cvNamedWindow("src", 1);
	cvShowImage("src", src);
	cvNamedWindow("dst", 1);
	cvShowImage("dst", dst);

	cvWaitKey(0);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	delete smoothFilter;

    return 0;
}
