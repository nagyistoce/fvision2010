Point Tracking
==============

:author ferryzhou@gmail.com
:version 0.1.0

Overview
--------

point_tracking package provides apis for tracking feature points on two views 
or multiple views. The package is based on SIFT and Pyramid LK methods. 


Examples
--------
1. two view point tracking:
	TwoViewPointTracker* tracker = TwoViewPointTrackerFactory().createSIFT();
	vector<CvPoint2D32f> x1s, x2s;
	tracker->compute(leftImage, rightImage, x1s, x2s);

2. multiple view point tracking:
	PointTracker* tracker = PointTrackerFactory().createPyrKLT();
	for (int frameId = 0; frameId < N; frameId++) tracker->addFrame(frameId, frame);
	const CvPoint2D32fDB* ptdb = tracker->getTrackingResults();


Requirements
------------

- opencv 1.0
- memdb 0.1
- Microsoft Visual Studio 2005 or newer
- gtest 1.4.0 (for development only)


