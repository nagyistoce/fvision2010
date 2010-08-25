image filter
============

:version 0.1.1
:author ferryzhou@gmail.com

Overview
--------
image filter package provides an object oriented api for image filtering. 
Basically, the image filter objects are first created and then used for image filtering.
Different type of filters can be easiy combined together to perform complex image processing tasks.


Examples
--------

	using namespace fvision::image_filters;	
	ImageFilter* filter = chain(smooth(), toGray(), toBinary(128));
	IplImage* dst = filter->process(src);


Requirements
------------

- opencv 1.0
- Microsoft Visual Studio 2005 or newer


Complie
-------

### Visual Studio 2005

visual stuido 2005 project files can be found in _make/vs2005. For convinience, I already include 
opencv 1.0 headers and libs in the project directory. There are two solution files: 
image_filter.sln and image_filter_samples.sln. Make sure compile image_filter before 
compiling image_filter_samples.


Tutorial
--------

### Basics ###

There are three interfaces in the package:
1. ImageFilter
2. SmartImageFilter
3. SequenceImageFilter

ImageFilter is the basic interface. It has a single pure virtual function:

	IplImage* process(IplImage* src, IplImage* buffer = NULL)

This function apply the filter to src and output the filtered image. The 'buffer' 
is the image to hold the filtering result. If it is NULL, then the function
will create a new image and return it; otherwise, the function write the output 
to the buffer and return buffer pointer.

To create an ImageFilter object, we need to use the functions in namespace 'image_filters', for instance:

	ImageFlter* filter = image_filters::smooth();
	
Actually, image_filters is the single entry point to create all ImageFilters, including
ImageFilter, SmartImageFilter and SequenceImageFilter. 

The following code shows an example to smooth an image:

	ImageFlter* filter = image_filters::smooth();
	IplImage* dst = smoothFilter->process(src);

For the complete example, please take a look at samples/smooth/image_filter_sample_smooth.cpp

### Sequential Filtering ###

Sometimes we want to apply several filtering to an image sequentially. 
For instance, we may want to first smooth the image, then convert the image to gray
scale and finally binarize it with some threshold value. To do this, we can use the 
following code:

	using namespace fvision::image_filters;	
	SequentialImageFilter* filter = chain();
	filter->addFilter(smooth());
	filter->addFilter(toGray());
	filter->addFilter(toBinary(128));
	IplImage* dst = filter->process(src);

Here we use 'SequentialImageFilter' to ease the task. 'SequentialImageFilter' is a 
special kind of 'ImageFilter' with the following interface:

	SequenceImageFilter* addFilter(ImageFilter* filter)
	
This function return 'this' pointer so we can rewrite previous code as:

	SequentialImageFilter* filter = image_filters::chain(smooth())
		->addFilter(toGray())->addFilter(toBinary(128));

A even easier way to is:

	using namespace fvision::image_filters;	
	SequentialImageFilter* filter = chain(smooth(), toGray(), toBinary(128));

### SmartImageFilter ###

Usually, an image filter is used many times on many images. To efficiently use memory, 
it's good to specify the buffer image when we call 'process' function. To specify the buffer
image, we need to know the output image's size and type. If the source images
have the same size, then this is not a problem. We only need to create the buffer 
image once. However, things become complex when the filter is going to be 
applied to many different type or differnt size images. Although we can manually 
check the source image and create the corresponding buffer image every time. This
is tedious. To ease the task, the package provide interface 'SmartImageFilter'. 
Basically, 'SmartImageFilter' is a special kind of ImageFilter which can automatically
 and intelligently manage the buffer object. When we call the 'process' function 
of a 'SmartImageFilter', we don't need to specify buffer object. Actually, we should not 
specify it. Otherwise, the function will report an exception. 'SmartImageFilter' is like
smart pointer. It can be created use the function smart():

	SmartImageFilter* smartFilter = image_filters::smart(filter)

	
