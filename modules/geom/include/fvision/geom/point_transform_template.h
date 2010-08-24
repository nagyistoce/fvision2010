#ifndef FVISION_POINT_TRANSFORM_TEMPLATE_H_
#define FVISION_POINT_TRANSFORM_TEMPLATE_H_

#include <fvision/cvutils.h>

#include <cv.h>

#include <cassert>
#include <vector>

/// point transformation templates
/// Point2D (CvPoint, CvPoint2D32f, CvPoint2D64f)
/// 1. transform with classes
///      Point2DTransformer<CvPoint>* t = point2d_transformers::homography<CvPoint>(H);
///      np = t->transform(p);
/// 2. transform with functions
///      np = transformPoint2D(H, p);
/// 3. copy transform a vector (use copyTransform in langutils)
///      nps = copyTransform(ps, *t);
/// 4. modify transform a vector (use modifyTransform in langutils)
///      modifyTransform(ps, *t);
/// 5. composite transformers (seems seldom used, so not implemented yet)
///      using namespace fvision::point2d_transformers;
///      Point2DTransformer<CvPoint>* t = chain(homography<CvPoint>(H), normalizer<CvPoint>(K))
/// 
/// transformers:
///    homography    (np = H * p)    /// most general 2d transform
///    translator    (np = p + t)
///    kNormalizer   (np = K * p)
///    kDenormalizer (np = inv(K) * p)
///    
/// Point3D
/// 
namespace fvision {

template<typename TPoint2D>
class Point2DTransformer {
public:
	virtual ~Point2DTransformer(void) {}
	virtual TPoint2D transform(const TPoint2D& p) const = 0;

	TPoint2D operator() (const TPoint2D& p) const {
		return transform(p);
	}
};

namespace point2d_transformers {

namespace internal {

template<typename TPoint2D>
class HomographyPoint2DTransformer : public Point2DTransformer<TPoint2D> {
public:
	HomographyPoint2DTransformer(const CvMat* H) {
		assert(H != NULL);
		assert(H->rows == 3 && H->cols == 3);
		this->H = cvCloneMat(H);
		this->inputPointBuffer = cvCreateMat(3, 1, CV_64FC1);
		this->outputPointBuffer = cvCreateMat(3, 1, CV_64FC1);
	}
	HomographyPoint2DTransformer(HomographyPoint2DTransformer& rhs) {
		if (this == &rhs) return;
		this->H = cvCloneMat(rhs.H);
		this->inputPointBuffer = cvCloneMat(rhs.inputPointBuffer);
		this->outputPointBuffer = cvCloneMat(rhs.outputPointBuffer);
	}
	~HomographyPoint2DTransformer(void) {
		cvReleaseMat(&H);
		cvReleaseMat(&inputPointBuffer);
		cvReleaseMat(&outputPointBuffer);
	}

public:
	TPoint2D transform(const TPoint2D& p) const {
		hmatFromPoint2D(p, inputPointBuffer);
		cvMatMul(H, inputPointBuffer, outputPointBuffer);
		TPoint2D np;
		hmatToPoint2D(outputPointBuffer, np);
		return np;
	}

private:
	CvMat* H;

	CvMat* inputPointBuffer;
	CvMat* outputPointBuffer;
};

template<typename TPoint2D>
class Point2DTranslator : public Point2DTransformer<TPoint2D> {
public:
	Point2DTranslator(const TPoint2D& t) { this->t = t; }
	TPoint2D transform(const TPoint2D& p) const { return p + t; }
private:
	CvPoint2D32f t;
};

//x' = invK * x
//x = ifx * (x - px), y = ify * (y - py)
template<typename TPoint2D>
class KNormalizer : public Point2DTransformer<TPoint2D> {
public:
	KNormalizer(const CvMat* K) {
		px = cvmGet(K, 0, 2);
		py = cvmGet(K, 1, 2);
		ifx = 1 / cvmGet(K, 0, 0);
		ify = 1 / cvmGet(K, 1, 1);
	}

	TPoint2D transform(const TPoint2D& p) const {
		TPoint2D np;
		np.x = ifx * (p.x - px);
		np.y = ify * (p.y - py);
		return np;
	}

private:
	double px, py, ifx, ify;
};

//x' = K * x
//x' = fx * x + px, y' = fy * y + py
template<typename TPoint2D>
class KDeNormalizer : public Point2DTransformer<TPoint2D> {
public:
	KDeNormalizer(const CvMat* K) {
		px = cvmGet(K, 0, 2);
		py = cvmGet(K, 1, 2);
		fx = cvmGet(K, 0, 0);
		fy = cvmGet(K, 1, 1);
	}

	TPoint2D transform(const TPoint2D& p) const {
		TPoint2D np;
		np.x = fx * p.x + px;
		np.y = fy * p.y + py;
		return np;
	}

private:
	double px, py, fx, fy;
};

} // end namespace point2d_transformers::internal

template<typename TPoint2D>
Point2DTransformer<TPoint2D>* homography(const CvMat* H) {
	return new internal::HomographyPoint2DTransformer<TPoint2D>(H);
}

template<typename TPoint2D>
Point2DTransformer<TPoint2D>* translator(const TPoint2D& t) {
	return new internal::Point2DTranslator<TPoint2D>(t);
}

template<typename TPoint2D>
Point2DTransformer<TPoint2D>* kNormalizer(const CvMat* K) {
	return new internal::KNormalizer<TPoint2D>(K);
}

template<typename TPoint2D>
Point2DTransformer<TPoint2D>* kDenormalizer(const CvMat* K) {
	return new internal::KDeNormalizer<TPoint2D>(K);
}

} // end namespace point2d_transformers


template<typename TPoint2D>
TPoint2D transformPoint2D(const CvMat* H, const TPoint2D& p) {
	point2d_transformers::internal::HomographyPoint2DTransformer<TPoint2D> transformer(H);
	return transformer.transform(p);
}

/// alias, for compatibility of old version
template<typename TPoint2D>
TPoint2D transformPoint(const CvMat* H, const TPoint2D& p) {
	return transformPoint2D<TPoint2D>(H, p);
}

template<typename TPoint2D>
std::vector<TPoint2D> transformPoints(const CvMat* H, const std::vector<TPoint2D>& ps) {
	std::vector<TPoint2D> nps(ps.size());
	point2d_transformers::internal::HomographyPoint2DTransformer<TPoint2D> transformer(H);
	transform(ps.begin(), ps.end(), nps.begin(), transformer);
	return nps;
}

template<typename TPoint2D>
std::vector<TPoint2D> normalizePointsWithCameraInternalMatrix(const CvMat* K, const std::vector<TPoint2D>& ps) {
	std::vector<TPoint2D> nps(ps.size());
	point2d_transformers::internal::KNormalizer<TPoint2D> transformer(K);
	transform(ps.begin(), ps.end(), nps.begin(), transformer);
	return nps;
}


/*
template<typename TPoint2D>
std::vector<TPoint2D> copyTransformPoint2DVector(const CvMat* H, const std::vector<TPoint2D>& ps) {
	HomographyPoint2DTransformer<TPoint2D> transformer(H);
	return copyTransform(ps, &transformer);
}

template<typename TPoint2D>
void modifyTransformPoint2DVector(const CvMat* H, std::vector<TPoint2D>& ps) {
	HomographyPoint2DTransformer<TPoint2D> transformer(H);
	modifyTransform(ps, &transformer);
}
*/
//======================= Point3D ==================================>

template<typename TPoint3D>
class Point3DTransformer {
public:
	virtual ~Point3DTransformer(void) {}
	virtual TPoint3D transform(const TPoint3D& p) const = 0;

	TPoint3D operator() (const TPoint3D& p) const {
		return transform(p);
	}
};

namespace point3d_transformers {

namespace internal {

template<typename TPoint3D>
class HomographyPoint3DTransformer : public Point3DTransformer<TPoint3D> {
public:
	HomographyPoint3DTransformer(const CvMat* H) {
		assert(H != NULL);
		assert(H->rows == 4 && H->cols == 4);
		this->H = cvCloneMat(H);
		this->inputPointBuffer = cvCreateMat(4, 1, CV_64FC1);
		this->outputPointBuffer = cvCreateMat(4, 1, CV_64FC1);
	}
	HomographyPoint3DTransformer(HomographyPoint3DTransformer& rhs) {
		if (this == &rhs) return;
		this->H = cvCloneMat(rhs.H);
		this->inputPointBuffer = cvCloneMat(rhs.inputPointBuffer);
		this->outputPointBuffer = cvCloneMat(rhs.outputPointBuffer);
	}
	~HomographyPoint3DTransformer(void) {
		cvReleaseMat(&H);
		cvReleaseMat(&inputPointBuffer);
		cvReleaseMat(&outputPointBuffer);
	}

public:
	TPoint3D transform(const TPoint3D& p) const {
		hmatFromPoint3D(p, inputPointBuffer);
		cvMatMul(H, inputPointBuffer, outputPointBuffer);
		TPoint3D np;
		hmatToPoint3D(outputPointBuffer, np);
		return np;
	}

private:
	CvMat* H;

	CvMat* inputPointBuffer;
	CvMat* outputPointBuffer;
};

} // end namespace point3d_transformers::internal

template<typename TPoint3D>
Point3DTransformer<TPoint3D>* homography(const CvMat* H) {
	return new internal::HomographyPoint3DTransformer<TPoint3D>(H);
}


}


template<typename TPoint3D>
TPoint3D transformPoint3D(const CvMat* H, const TPoint3D& p) {
    point3d_transformers::internal::HomographyPoint3DTransformer<TPoint3D> transformer(H);
	return transformer.transform(p);
}
/*
template<typename TPoint3D>
std::vector<TPoint3D> copyTransformPoint3DVector(const CvMat* H, const std::vector<TPoint3D>& ps) {
	HomographyPoint3DTransformer<TPoint3D> transformer(H);
	return copyTransform(ps, &transformer);
}
*/
template<typename TPoint3D>
void modifyTransformPoint3DVector(const CvMat* H, std::vector<TPoint3D>& ps) {
	Point3DTransformer<TPoint3D>* transformer = point3d_transformers::homography<TPoint3D>(H);
	for (size_t i = 0; i < ps.size(); i++) ps[i] = transformer->transform(ps[i]);
	delete transformer;
}


}
#endif // FVISION_POINT_TRANSFORM_TEMPLATE_H_
