#ifndef FVISION_POINT_ARITH_H_
#define FVISION_POINT_ARITH_H_

#include <cv.h>
#include <vector>

//operator overloading

namespace fvision {

CvPoint operator+ (const CvPoint& p1, const CvPoint& p2);
CvPoint2D32f operator+ (const CvPoint2D32f& p1, const CvPoint2D32f& p2);
CvPoint operator- (const CvPoint& p1, const CvPoint& p2);
CvPoint2D32f operator- (const CvPoint2D32f& p1, const CvPoint2D32f& p2);
CvPoint operator* (const CvPoint& p1, double v);
CvPoint2D32f operator* (const CvPoint2D32f& p1, double v);
CvPoint operator/ (const CvPoint& p1, double v);
CvPoint2D32f operator/ (const CvPoint2D32f& p1, double v);

std::vector<CvPoint2D32f> operator+ (const std::vector<CvPoint2D32f>& pts, const CvPoint2D32f& tp);


CvSize operator* (const CvSize& size, double v);
CvSize operator/ (const CvSize& size, double v);

CvRect operator* (const CvRect& rect, double v);
CvRect operator/ (const CvRect& rect, double v);


//========= Operators for CvPoint3D32f ===================>
CvPoint3D32f operator+ (const CvPoint3D32f& p1, const CvPoint3D32f& p2);
CvPoint3D32f operator- (const CvPoint3D32f& p1, const CvPoint3D32f& p2);
CvPoint3D32f operator* (const CvPoint3D32f& p1, double v);
CvPoint3D32f operator/ (const CvPoint3D32f& p1, double v);

std::vector<CvPoint3D32f> operator* (const std::vector<CvPoint3D32f>& ps, double v);

/** Dot product
*/
double operator* (const CvPoint3D32f& p1, const CvPoint3D32f& p2);

//========= Operators for CvPoint3D64f ===================>
CvPoint3D64f operator+ (const CvPoint3D64f& p1, const CvPoint3D64f& p2);
CvPoint3D64f operator- (const CvPoint3D64f& p1, const CvPoint3D64f& p2);
CvPoint3D64f operator* (const CvPoint3D64f& p1, double v);
CvPoint3D64f operator/ (const CvPoint3D64f& p1, double v);

}

#endif // FVISION_POINT_ARITH_H_