#include "point_arith.h"

using namespace std;

namespace fvision {

CvPoint operator+ (const CvPoint& p1, const CvPoint& p2) {
	return cvPoint(p1.x + p2.x, p1.y + p2.y);
}

CvPoint2D32f operator+ (const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
	return cvPoint2D32f(p1.x + p2.x, p1.y + p2.y);
}

CvPoint operator- (const CvPoint& p1, const CvPoint& p2) {
	return cvPoint(p1.x - p2.x, p1.y - p2.y);
}

CvPoint2D32f operator- (const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
	return cvPoint2D32f(p1.x - p2.x, p1.y - p2.y);
}

CvPoint operator* (const CvPoint& p1, double v) {
	return cvPoint((int)(p1.x * v), (int)(p1.y * v));
}

CvPoint2D32f operator* (const CvPoint2D32f& p1, double v) {
	return cvPoint2D32f(p1.x * v, p1.y * v);
}

CvPoint operator/ (const CvPoint& p1, double v) {
	return cvPoint((int)(p1.x / v), (int)(p1.y / v));
}

CvPoint2D32f operator/ (const CvPoint2D32f& p1, double v) {
	return cvPoint2D32f(p1.x / v, p1.y / v);
}

CvSize operator* (const CvSize& size, double v) {
	return cvSize((int)(size.width * v), (int)(size.height * v));
}

CvSize operator/ (const CvSize& size, double v) {
	return cvSize(size.width / 2, size.height / 2);
}

vector<CvPoint2D32f> operator+ (const vector<CvPoint2D32f>& pts, const CvPoint2D32f& tp) {
	vector<CvPoint2D32f> npts;
	for (size_t i = 0; i < pts.size(); i++) {
		npts.push_back(pts[i] + tp);
	}
	return npts;
}



CvRect operator* (const CvRect& rect, double v) {
	return cvRect((int)(rect.x * v), (int)(rect.y * v), (int)(rect.width * v), (int)(rect.height * v));
}

CvRect operator/ (const CvRect& rect, double v) {
	return cvRect((int)(rect.x / v), (int)(rect.y / v), (int)(rect.width / v), (int)(rect.height / v));
}

CvPoint3D32f operator* (const CvPoint3D32f& p1, double v) {
	return cvPoint3D32f(p1.x * v, p1.y * v, p1.z * v);
}

vector<CvPoint3D32f> operator* (const vector<CvPoint3D32f>& ps, double v) {
	vector<CvPoint3D32f> nps;
	for (size_t i = 0; i < ps.size(); i++) {
		nps.push_back(ps[i] * v);
	}
	return nps;
}

CvPoint3D32f operator+ (const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
	return cvPoint3D32f(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

CvPoint3D32f operator- (const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
	return cvPoint3D32f(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

CvPoint3D32f operator/ (const CvPoint3D32f& p1, double v) {
	return cvPoint3D32f(p1.x / v, p1.y / v, p1.z / v);
}

double operator* (const CvPoint3D32f& p1, const CvPoint3D32f& p2) {
	return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

//========= Operators for CvPoint3D32f ===================>
CvPoint3D64f operator+ (const CvPoint3D64f& p1, const CvPoint3D64f& p2) {
	return cvPoint3D64f(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

CvPoint3D64f operator- (const CvPoint3D64f& p1, const CvPoint3D64f& p2) {
	return cvPoint3D64f(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

CvPoint3D64f operator* (const CvPoint3D64f& p1, double v) {
	return cvPoint3D64f(p1.x * v, p1.y * v, p1.z * v);
}

CvPoint3D64f operator/ (const CvPoint3D64f& p1, double v) {
	return cvPoint3D64f(p1.x / v, p1.y / v, p1.z / v);
}

}