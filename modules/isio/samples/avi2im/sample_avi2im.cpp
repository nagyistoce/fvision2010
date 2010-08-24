#include <fvision/isio.h>
#include <iostream>
#include <cstdio>

using namespace fvision;
using namespace std;

int main(int argc, char* argv[]) {

	if (argc < 3) { cout<<"please input the avi path and then a folder path"<<endl; exit(1); }
	char pathRegex[500]; sprintf(pathRegex, "%s/im_%%05d.jpg", argv[2]);

	ImageSequenceReaderFactory readerFactory;
	ImageSequenceReader* reader = readerFactory.avi(argv[1]);
	if (reader == NULL) { cout<<"error in open avi: "<<argv[1]<<endl; exit(1); }
	ImageSequenceWriter* writer = ImageSequenceWriterFactory().pathRegex(pathRegex, 0);
	isio::run(reader, writer);

	delete reader;
	delete writer;

    return 0;
}
