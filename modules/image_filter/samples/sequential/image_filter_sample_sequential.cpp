#include <fvision/image_filter.h>

#include <highgui.h>

#include <iostream>

using namespace std;
using namespace fvision;

int main(int argc, char* argv[]) {

	using namespace fvision::image_filters;
	
	SequentialImageFilter* filter = chain(smooth(), toGray(), toBinary(128));

	IplImage* src = cvLoadImage(argv[1]);

	if (src == NULL) { cout<<"fail to load image: ["<<argv[1]<<"]"<<endl; }

	IplImage* dst = filter->process(src);

	cvNamedWindow("src", 1);
	cvShowImage("src", src);
	cvNamedWindow("dst", 1);
	cvShowImage("dst", dst);

	cvWaitKey(0);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);

	delete filter;

    return 0;
}
