calculators
===========

Overview
--------

A set of geometric model calculators are implemented based on moca framework:

   MODEL                EQUATION         SAMPLE TYPE
homography (H)          x' ~= H x         <x, x'>
fundamental matrix (F)  x'^T F x ~= 0     <x, x'>
camera (P)              x ~= P X          <x, X>
point  (X)              x ~= P X          <x, P>
line intersection (x)   l^T x = 0         <l>


Examples
--------
1. Homography

	PointPairs pps;
	....
	HomographyCalculator* ransac = HomographyCalculatorFactory().createRansac(threshold);
	CvMatWrapper H = ransac->compute(pps);
	
	HomographyGeometricErrorEvaluator* fmec = new HomographyGeometricErrorEvaluator(threshold);
	HomographyEvaluationResults fResults = fmec->compute(H.getMat(), pps);

	if (verbose) {
		cout<<"inliers error: "<<fResults.averageInlierError<<endl;
		cout<<"inliers rate:"<<fResults.getInliersRate()<<endl;
	}

	PointPairs ipps = fResults.inlierSamples;
	PointPairs opps = fResults.inlierSamples;


Requirements
------------

- geom
- cvutils
- moca
- opencv

