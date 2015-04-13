fvision2010 is an image processing and 3D vision library built upon OpenCV.

This site replaces the previous code base [fvision2008](http://code.google.com/p/fvision2008/).

current available packages:

  * image\_filter 0.1.1
```
	using namespace fvision::image_filters;	
	ImageFilter* filter = chain(smooth(), toGray(), toBinary(128));
	IplImage* dst = filter->process(src);
```

  * isio 0.1.1  --- _image sequence input output_
```
	ImageSequenceReaderFactory factory;
	ImageSequenceReader* reader = factory.pathRegex("c:/a/im_%03d.jpg", 0, 20);
	//ImageSequenceReader* reader = factory.avi("a.avi");
	if (reader == NULL) { cout<<factory.getErrorMessage()<<endl; exit(1); }
	IplImage* frame = NULL;
	while ((frame = reader->read()) != NULL) {
		process(frame);
	}
	delete reader;
```

  * point\_tracking
```
1. two view point tracking:
	TwoViewPointTracker* tracker = TwoViewPointTrackerFactory().createSIFT();
	vector<CvPoint2D32f> x1s, x2s;
	tracker->compute(leftImage, rightImage, x1s, x2s);

2. multiple view point tracking:
	PointTracker* tracker = PointTrackerFactory().createPyrKLT();
	for (int frameId = 0; frameId < N; frameId++) tracker->addFrame(frameId, frame);
	const CvPoint2D32fDB* ptdb = tracker->getTrackingResults();

```

  * cvutils  --- _utils for c++ usage of OpenCV_
  * geom  --- _geometric utils_
  * moca  --- _model calculation (DLT/RANSAC) and evaluation framework, used by calculators_
  * calculators  --- _calculators of geometric models such as homography, fundamental matrix_
```
1. Homograhy

	PointPairs pps;
	......
	HomographyCalculator* ransac = HomographyCalculatorFactory().createRansac(threshold);
	CvMatWrapper H = ransac->compute(pps);
	
	HomographyGeometricErrorEvaluator* fmec = new HomographyGeometricErrorEvaluator(threshold);
	HomographyEvaluationResults fResults = fmec->compute(H.getMat(), pps);

	if (verbose) {
		cout<<"inliers error: "<<fResults.averageInlierError<<endl;
		cout<<"inliers rate:"<<fResults.getInliersRate()<<endl;
	}

	PointPairs ipps = fResults.inlierSamples;
	PointPairs opps = fResults.outlierSamples;


```
  * cvdraw   --- _drawing classes and functions based on OpenCV_
  * memdb   --- _in memory database indexed by object ID or ID pairs_
  * langutils   --- _c++ language utils such as math, templates_