Image Sequence Input Output
===========================

:version 0.1.2
:author ferryzhou@gmail.com

Overview
--------
isio package provides a convenient way for image sequence input and output.


Examples
--------

	ImageSequenceReaderFactory factory;
	ImageSequenceReader* reader = factory.pathRegex("c:/a/im_%03d.jpg", 0, 20);
	//ImageSequenceReader* reader = factory.avi("a.avi");
	if (reader == NULL) { cout<<factory.getErrorMessage()<<endl; exit(1); }
	IplImage* frame = NULL;
	while ((frame = reader->read()) != NULL) {
		process(frame);
	}
	delete reader;


Requirements
------------

- opencv 1.0
- Microsoft Visual Studio 2005 or newer


Complie
-------

### Visual Studio 2005

visual stuido 2005 project files can be found in _make/vc8. 


Tutorial
--------

### Basics ###

There are four interfaces in the package:
1. ImageSequenceReader
2. ImageSequenceWriter
3. ImageSequenceReaderFactory
4. ImageSequenceWriterFactory

The meaning of each interface is just as the name says. Basically, ImageSequenceReader 
can only be created by ImageSequenceReaderFactory and ImageSequenceWriter can only be 
created by ImageSequenceWriterFactory. Currently, only two kinds of reader/writer are 
provided, i.e. pathRegex and avi

The following code gives an example to create a reader with path regex string:

	ImageSequenceReaderFactory factory;
	ImageSequenceReader* reader = factory.pathRegex("c:/a/im_%03d.jpg", 0, 20);

The reader will read from c:/a/im_000.jpg and end with c:/a/im_019.jpg.

ImageSequenceReader has two interfaces:

	virtual IplImage* read() = 0;
	virtual bool reset() = 0;

read() can retrieve the next image. If a NULL is returned, then no more image can be read.
reset() set the reader to the beginning status. 

ImageSequenceWriter has two interfaces too:

	virtual bool write(const IplImage* im) = 0;
	virtual const char* getErrorMessage() const { return ""; }

