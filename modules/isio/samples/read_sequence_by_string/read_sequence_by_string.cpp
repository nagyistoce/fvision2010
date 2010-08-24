#include <fvision/isio.h>
#include <iostream>
#include <highgui.h>

using namespace fvision;
using namespace std;

int main(int argc, char* argv[]) {

	if (argc < 2) { cout<<"please specify an image sequence path (either avi, folder or an image)"<<endl; exit(1); }

	ImageSequenceReaderFactory readerFactory;
	ImageSequenceReader* reader = readerFactory.createFromString(argv[1]);
	if (reader == NULL) { cout<<"error in loading: "<<argv[1]<<endl; exit(1); }

	cvNamedWindow("frame");
	IplImage* frame = NULL;
	while ((frame = reader->read()) != NULL) {
		cvShowImage("frame", frame);
		cvWaitKey(30);
	}

	delete reader;
	cvWaitKey(0);

    return 0;
}
