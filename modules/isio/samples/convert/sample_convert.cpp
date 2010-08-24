#include <fvision/isio.h>
#include <iostream>

using namespace fvision;
using namespace std;

int main(int argc, char* argv[]) {

	if (argc < 3) { cout<<"please specify an input path and a output path"<<endl; exit(1); }

	ImageSequenceReaderFactory readerFactory;
	ImageSequenceReader* reader = readerFactory.createFromString(argv[1]);
	if (reader == NULL) { cout<<"error in loading: "<<argv[1]<<endl; exit(1); }

	ImageSequenceWriterFactory writerFactory;
	ImageSequenceWriter* writer = writerFactory.avi(argv[2]);
	if (writer == NULL) { cout<<"error in loading: "<<argv[2]<<endl; exit(1); }

	isio::run(reader, writer);

	delete reader;
	delete writer;

    return 0;
}
