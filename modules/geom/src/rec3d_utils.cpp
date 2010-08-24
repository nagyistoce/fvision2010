#include "rec3d_utils.h"

#include "singleviewgeom.h"
#include "camera_utils.h"
//#include "projection_template.h"

#include <fvision/cvutils.h>

#include <iostream>
#include <algorithm>
using namespace std;

namespace fvision {

namespace rec3d_internal {

	double getMean(const vector<double>& vs) {
		int times = (int)vs.size();
		if (times == 0) {
			return 0;
		}

		double sum = 0;
		for (int i = 0; i < times; i++) {
			sum += vs[i];
		}
		return sum / times;
	}

}

CvMat* triangulate(const CvMat* P1, 
				   const CvMat* P2, 
				   const CvPoint2D32f& x1, 
				   const CvPoint2D32f& x2) 
{
	
	CvMat* A = cvCreateMat(4, 4, CV_64FC1);
	
	CvMat* p1t = getRow(P1, 0);
	CvMat* p2t = getRow(P1, 1);
	CvMat* p3t = getRow(P1, 2);
	CvMat* xp3t = scale(p3t, x1.x);
	CvMat* a1 = sub(xp3t, p1t);
	CvMat* yp3t = scale(p3t, x1.y);
	CvMat* a2 = sub(yp3t, p2t);

	CvMat* p1t2 = getRow(P2, 0);
	CvMat* p2t2 = getRow(P2, 1);
	CvMat* p3t2 = getRow(P2, 2);
	CvMat* xp3t2 = scale(p3t2, x2.x);
	CvMat* a3 = sub(xp3t2, p1t2);
	CvMat* yp3t2 = scale(p3t2, x2.y);
	CvMat* a4 = sub(yp3t2, p2t2);

	setRow(A, a1, 0);
	setRow(A, a2, 1);
	setRow(A, a3, 2);
	setRow(A, a4, 3);
	
	cvReleaseMat(&p1t);
	cvReleaseMat(&p2t);
	cvReleaseMat(&p3t);
	cvReleaseMat(&xp3t);
	cvReleaseMat(&yp3t);

	cvReleaseMat(&p1t2);
	cvReleaseMat(&p2t2);
	cvReleaseMat(&p3t2);
	cvReleaseMat(&xp3t2);
	cvReleaseMat(&yp3t2);

	cvReleaseMat(&a1);
	cvReleaseMat(&a2);
	cvReleaseMat(&a3);
	cvReleaseMat(&a4);

	return null(A);
}

Point3D triangulateToPoint3D(const CvMat* P1, 
							 const CvMat* P2, 
							 const CvPoint2D32f& x1, 
							 const CvPoint2D32f& x2) 
{
	CvMat* hm = triangulate(P1, P2, x1, x2);
	Point3D p;
	hmatToPoint3D(hm, p);
	cvReleaseMat(&hm);
	return p;
}

vector<Point3D> triangulateToPoint3Ds(const CvMat* P1, 
									  const CvMat* P2, 
									  const vector<CvPoint2D32f>& x1s, 
									  const vector<CvPoint2D32f>& x2s) 
{
	vector<Point3D> xs;
	for (size_t i = 0; i < x1s.size(); i++) {
		xs.push_back(triangulateToPoint3D(P1, P2, x1s[i], x2s[i]));
	}
	return xs;
}

double triangulationError(const Point3D& X, 
						  const CvMat* P1, 
						  const CvMat* P2, 
						  const CvPoint2D32f& x1, 
						  const CvPoint2D32f& x2, 
						  int metric) 
{
	CvPoint2D32f px1 = projectWithP(P1, X);
	CvPoint2D32f px2 = projectWithP(P2, X);
	if (metric == 1) return (pointDistance(px1, x1) + pointDistance(px2, x2)) / 2;
	else return (pointDistanceSquare(px1, x1) + pointDistanceSquare(px2, x2)) / 2;
}

double getReprojectionError(const Point3D& X, 
							const CvMat* P, 
							const CvPoint2D32f& x, 
							int metric) 
{
	CvPoint2D32f PX = projectWithP(P, X);
	if (metric == 1) return pointDistance(PX, x);
	else return pointDistanceSquare(PX, x);
}

std::vector<double> getReprojectionErrors(const Point3D& X, 
										  const std::vector<CvMat*>& Ps, 
										  const std::vector<CvPoint2D32f>& xs, 
										  int metric /*= 1*/) 
{
	assert(Ps.size() == xs.size());
	vector<double> errors(xs.size());
	for (size_t i = 0; i < xs.size(); i++) {
		errors[i] = getReprojectionError(X, Ps[i], xs[i], metric);
	}
	return errors;
}

double getAverageReprojectionError(const Point3D& X, 
								   const vector<CvMat*>& Ps, 
								   const vector<CvPoint2D32f>& xs, 
								   int metric) 
{
	assert(Ps.size() == xs.size());
	vector<double> errors = getReprojectionErrors(X, Ps, xs, metric);
	return rec3d_internal::getMean(errors);
}

vector<double> triangulationErrors(const vector<Point3D>& Xs, 
								   const CvMat* P1, 
								   const CvMat* P2, 
								   const vector<CvPoint2D32f>& x1s, 
								   const vector<CvPoint2D32f>& x2s, 
								   int metric) 
{
	assert(Xs.size() == x1s.size() && Xs.size() == x2s.size());
	vector<double> errors;
	for (size_t i = 0; i < Xs.size(); i++) {
		errors.push_back(triangulationError(Xs[i], P1, P2, x1s[i], x2s[i], metric));
	}
	return errors;
}

//E = [t]xR
//t = (-)u3, R = UW(t)Vt
//four solutions
//only one is valid
//validate through most valid points
void computeRt(const CvMat* E, 
			   const vector<CvPoint2D32f>& nx1s, 
			   const vector<CvPoint2D32f>& nx2s, 
			   CvMat* R, 
			   CvMat* t) 
{

	CvMat* U;
	CvMat* V;
	CvMat* D;
	svd(E, &D, &U, &V); //E = U*D*V'

	//t1 = u3, last column of U
	CvMat* t1 = getCol(U, 2);
	CvMat* t2 = scale(t1, -1);

	CvMat* W = cvCreateMat(3, 3, CV_64FC1);
	cvZero(W);
	cvmSet(W, 0, 1, -1);
	cvmSet(W, 1, 0, 1);
	cvmSet(W, 2, 2, 1);

	CvMat* Vt = transpose(V);
	CvMat* Wt = transpose(W);
	CvMat* R1 = matMul(U, W, Vt);
	CvMat* R2 = matMul(U, Wt, Vt);
	
	//normalize
	//cout<<"R1: "<<R1<<endl;
	//cout<<"R2: "<<R2<<endl;
	if (cvDet(R1) < 0) cvScale(R1, R1, -1);
	if (cvDet(R2) < 0) cvScale(R2, R2, -1);
	//cout<<"after normalize..."<<endl;
	//cout<<"R1: "<<R1<<endl;
	//cout<<"R2: "<<R2<<endl;

	vector<int> validnum;
	validnum.push_back(computeValidPointsNum(R1, t1, nx1s, nx2s));
	validnum.push_back(computeValidPointsNum(R1, t2, nx1s, nx2s));
	validnum.push_back(computeValidPointsNum(R2, t1, nx1s, nx2s));
	validnum.push_back(computeValidPointsNum(R2, t2, nx1s, nx2s));

	int maxIndex = max_element(validnum.begin(), validnum.end()) - validnum.begin();

	switch (maxIndex) {
		case 0: { cvCopy(R1, R); cvCopy(t1, t); break; }
		case 1: { cvCopy(R1, R); cvCopy(t2, t); break; }
		case 2: { cvCopy(R2, R); cvCopy(t1, t); break; }
		case 3: { cvCopy(R2, R); cvCopy(t2, t); break; }
		default: { cvCopy(R1, R); cvCopy(t1, t); }
	}

	//post condition
	//[t]xR = E

	//clean up
	cvReleaseMat(&U);
	cvReleaseMat(&V);
	cvReleaseMat(&D);
	cvReleaseMat(&W);
	cvReleaseMat(&Vt);
	cvReleaseMat(&Wt);
	cvReleaseMat(&t1);
	cvReleaseMat(&t2);
	cvReleaseMat(&R1);
	cvReleaseMat(&R2);
}

int computeValidPointsNum(CvMat* R, 
						  CvMat* t, 
						  const vector<CvPoint2D32f>& nx1s, 
						  const vector<CvPoint2D32f>& nx2s) 
{
	int n = 0;
	CvMat* NP1 = getStandardNP1();
	CvMat* NP2 = getNP(R, t);
	//cout<<"NP2: "<<NP2<<endl;
	for (unsigned int i = 0; i < nx1s.size(); i++) {
		if (isValid(NP1, NP2, nx1s[i], nx2s[i])) n++;
	}
	//cout<<"valid num: "<<n<<endl;
	return n;
}

bool isValid(CvMat* P1, 
			 CvMat* P2, 
			 const CvPoint2D32f& x1, 
			 const CvPoint2D32f& x2) 
{
	CvMat* mX = triangulate(P1, P2, x1, x2);
	if (cvmGet(mX, 3, 0) < 0) cvScale(mX, mX, -1);
	CvMat* mx1 = matMul(P1, mX);
	CvMat* mx2 = matMul(P2, mX);
	bool valid = (cvmGet(mx1, 2, 0) > 0) && (cvmGet(mx2, 2, 0) > 0);
	cvReleaseMat(&mX);
	cvReleaseMat(&mx1);
	cvReleaseMat(&mx2);
	return valid;
}

//proj_x1s = P1 * Xs, proj_x2s = P2 * Xs
//error = sum_error(proj_x1s, x1s) + sum_error(proj_x2s, x2s);
double computeReprojectionError(CvMat* P1, 
								CvMat* P2, 
								const vector<CvPoint2D32f>& x1s, 
								const vector<CvPoint2D32f>& x2s, 
								const vector<Point3D>& Xs) 
{
	vector<CvPoint2D32f> proj_x1s = projectWithP(P1, Xs);
	vector<CvPoint2D32f> proj_x2s = projectWithP(P2, Xs);
	return averagePointDistanceSquare(proj_x1s, x1s) + averagePointDistanceSquare(proj_x2s, x2s);
}

//
CvMat* batchTriangulate(const vector<CvMat*>& Ps, 
						const vector<CvPoint2D32f> xs, 
						const CvMat* initmX) 
{

	assert(Ps.size() == xs.size() && xs.size() >= 2);

	int size = (int)xs.size();

	//2n x 4
	CvMat* A = cvCreateMat(2 * size, 4, CV_64FC1);
	
	CvMat* p1t = cvCreateMat(1, 4, CV_64FC1);
	CvMat* p2t = cvCreateMat(1, 4, CV_64FC1);
	CvMat* p3t = cvCreateMat(1, 4, CV_64FC1);
	CvMat* xp3t = cvCreateMat(1, 4, CV_64FC1);
	CvMat* yp3t = cvCreateMat(1, 4, CV_64FC1);
	CvMat* a1 = cvCreateMat(1, 4, CV_64FC1);
	CvMat* a2 = cvCreateMat(1, 4, CV_64FC1);

	CvMat* initmXt = NULL; 
	if (initmX != NULL) initmXt = transpose(initmX);

	for (int i = 0; i < size; i++) {
		cvGetRow(Ps[i], p1t, 0);
		cvGetRow(Ps[i], p2t, 1);
		cvGetRow(Ps[i], p3t, 2);

		cvScale(p3t, xp3t, xs[i].x);
		cvSub(xp3t, p1t, a1);

		cvScale(p3t, yp3t, xs[i].y);
		cvSub(yp3t, p2t, a2);

		double invDepth = 1;
        if (initmXt != NULL) invDepth = 1 / fabs(cvDotProduct(p3t, initmXt));

		cvScale(a1, a1, invDepth);
		cvScale(a2, a2, invDepth);

		setRow(A, a1, 2 * i);
		setRow(A, a2, 2 * i + 1);
	}

	cvReleaseMat(&p1t);
	cvReleaseMat(&p2t);
	cvReleaseMat(&p3t);
	cvReleaseMat(&xp3t);
	cvReleaseMat(&yp3t);
	cvReleaseMat(&a1);
	cvReleaseMat(&a2);
	cvReleaseMat(&initmXt);

	return null(A);
}

Point3D batchTriangulate(const vector<CvMat*>& Ps, 
					const vector<CvPoint2D32f> xs, 
					const Point3D& initX) 
{
	CvMat* initmX = hmatFromPoint3D(initX);
	CvMat* mX = batchTriangulate(Ps, xs, initmX);
	Point3D X;
	hmatToPoint3D(mX, X);
	cvReleaseMat(&initmX);
	cvReleaseMat(&mX);
	return X;
}

fvision::Point3D batchTriangulate(const std::vector<CvMat*>& Ps, const std::vector<CvPoint2D32f> xs) {
	CvMat* mX = batchTriangulate(Ps, xs, NULL);
	Point3D X;
	hmatToPoint3D(mX, X);
	cvReleaseMat(&mX);
	return X;	
}

}
