#include "misc.h"

//#include <fvision/langutils.h>

#include <algorithm>

using namespace std;

namespace fvision {

	namespace cvutils_internal {
	float ranf() {
		float d = (float) rand() / RAND_MAX;
		return d;
	}

	int rani(int max) {
		return (int)(ranf() * max - 0.5);
	}

	float gaussianRand(float m, float s)	/* normal random variate generator */
	{				        /* mean m, standard deviation s */
		float x1, x2, w, y1;
		static float y2;
		static int use_last = 0;

		if (use_last)		        /* use value from previous call */
		{
			y1 = y2;
			use_last = 0;
		}
		else
		{
			do {
				x1 = 2 * ranf() - 1;
				x2 = 2 * ranf() - 1;
				w = x1 * x1 + x2 * x2;
			} while ( w >= 1 );

			w = sqrt( (-2 * log( w ) ) / w );
			y1 = x1 * w;
			y2 = x2 * w;
			use_last = 1;
		}

		return( m + y1 * s );
	}

	vector<int> ranis(int max, int num) {
		assert(max > 0);
		vector<int> is;
		if (num >= max) {
			for (int i = 0; i < max; i++) {
				is.push_back(i);
			}
		} else {
			for (int i = 0; i < num; ) {
				int v = rani(max);
				if (find(is.begin(), is.end(), v) == is.end()) {
					is.push_back(v);
					++i;
				}
			}
		}
		return is;
	}

	}

vector<CvPoint> cvPointsFrom32fs(const vector<CvPoint2D32f>& pts) {
	vector<CvPoint> ps;
	for (int i = 0; i < (int)pts.size(); i++) {
		ps.push_back(cvPointFrom32f(pts[i]));
	}
	return ps;
}

vector<CvPoint2D32f> cvPointsTo32fs(const vector<CvPoint>& pts) {
	vector<CvPoint2D32f> ps;
	for (int i = 0; i < (int)pts.size(); i++) {
		ps.push_back(cvPointTo32f(pts[i]));
	}
	return ps;
}

CvPoint3D32f cvPoint3D32fFrom64f(const CvPoint3D64f& p) {
	return cvPoint3D32f(p.x, p.y, p.z);
}

CvPoint3D64f cvPoint3D32fTo64f(const CvPoint3D32f& p) {
	return cvPoint3D64f(p.x, p.y, p.z);
}

vector<CvPoint3D32f> cvPoint3D32fsFrom64fs(const vector<CvPoint3D64f>& pts) {
	vector<CvPoint3D32f> npts(pts.size());
	transform(pts.begin(), pts.end(), npts.begin(), cvPoint3D32fFrom64f);
	return npts;
}

vector<CvPoint3D64f> cvPoint3D32fsTo64fs(const vector<CvPoint3D32f>& pts) {
	vector<CvPoint3D64f> npts(pts.size());
	transform(pts.begin(), pts.end(), npts.begin(), cvPoint3D32fTo64f);
	return npts;
}

void randomlySampleNPointPairs(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, 
	vector<CvPoint2D32f>& sx1s, vector<CvPoint2D32f>& sx2s, int n) {

	vector<int> is = cvutils_internal::ranis((int)x1s.size(), n);
	for (int i = 0; i < n; i++) {
		int index = is[i];
		sx1s.push_back(x1s[index]);
		sx2s.push_back(x2s[index]);
	}
}

CvPoint2D32f addNoiseToPoint(float standardDeviation, const CvPoint2D32f& pt) {
	using namespace fvision::cvutils_internal;
	return cvPoint2D32f(pt.x + gaussianRand(0, standardDeviation), pt.y + gaussianRand(0, standardDeviation));
}

vector<CvPoint2D32f> addNoiseToPoints(float standardDeviation, vector<CvPoint2D32f>& pts) {
	using namespace fvision::cvutils_internal;
	vector<CvPoint2D32f> npts;
	for (int i = 0; i < (int)pts.size(); i++) {
		npts.push_back(cvPoint2D32f(pts[i].x + gaussianRand(0, standardDeviation), pts[i].y + gaussianRand(0, standardDeviation)));
	}
	return npts;
}

std::vector<double> addNoiseToPoints(float standardDeviation, std::vector<double>& pts) {
	using namespace fvision::cvutils_internal;
	vector<double> npts(pts.size());
	for (int i = 0; i < (int)pts.size(); i++) {
		npts[i] = (double)gaussianRand((float)pts[i], standardDeviation);
	}
	return npts;
}

CvPoint2D32f cvPoint2D32fFrom64f(const CvPoint2D64f& p) {
	return cvPoint2D32f((float)p.x, (float)p.y);
}

std::vector<double> getDisparities(const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s) {
	assert(x1s.size() == x2s.size());
	vector<double> ds(x1s.size());
	for (size_t i = 0; i < x1s.size(); i++) {
		ds[i] = x2s[i].x - x1s[i].x;
	}
	return ds;
}

std::vector<int> createIntVector(int from, int to, int step /*= 1*/) {
	assert (step * (to - from) > 0); //same direction
	vector<int> vs;
	for (int i = from; i < to; i += step) {
		vs.push_back(i);
	}
	return vs;
}
}
