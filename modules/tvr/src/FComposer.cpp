#include "fcomposer.h"

#include <fvision/cvutils.h>
#include <fvision/geom.h>

#include <iostream>

using namespace std;

namespace fvision {

namespace fcomposer_internal {

extern double PI = 3.1415926;

double degreesToRadian(double degrees) {
	return degrees * PI / 180;
}

double radianToDegrees(double radian) {
	return radian * 180 / PI;
}
}


ostream& operator<<(ostream& os, const FParams& fp) {
	os<<fp.f1<<endl;
	os<<fp.f2<<endl;
	os<<fcomposer_internal::radianToDegrees(fp.ztheta1)<<endl;
	os<<fcomposer_internal::radianToDegrees(fp.ztheta2)<<endl;
	os<<fcomposer_internal::radianToDegrees(fp.ytheta1)<<endl;
	os<<fcomposer_internal::radianToDegrees(fp.ytheta2)<<endl;
	os<<fcomposer_internal::radianToDegrees(fp.xtheta1)<<endl;
	return os;
}

istream& operator>>(istream& is, FParams& fp) {
	is>>fp.f1>>fp.f2;
	double degrees = 0;
	is>>degrees;
	fp.ztheta1 = fcomposer_internal::degreesToRadian(degrees);
	is>>degrees;
	fp.ztheta2 = fcomposer_internal::degreesToRadian(degrees);
	is>>degrees;
	fp.ytheta1 = fcomposer_internal::degreesToRadian(degrees);
	is>>degrees;
	fp.ytheta2 = fcomposer_internal::degreesToRadian(degrees);
	is>>degrees;
	fp.xtheta1 = fcomposer_internal::degreesToRadian(degrees);
	return is;
}

FComposer::FComposer() {
	FParams params;
	init(params);
}

FComposer::FComposer(const FParams& params) {
	init(params);
}

FComposer::FComposer(const FComposer& fc) {
	init(fc.params);
}

FComposer& FComposer::operator=(const FComposer& rhs) {
	if (this == &rhs) return *this;
	clearData();
	init(rhs.params);
	return *this;
}

FComposer::~FComposer() {
	clearData();
}

void FComposer::clearData() {
	clearMat(&K1);
	clearMat(&K2);
	clearMat(&rz1);
	clearMat(&rz2);
	clearMat(&ry1);
	clearMat(&ry2);
	clearMat(&rx1);
	clearMat(&F);
	clearMat(&H1);
	clearMat(&H2);
	clearMat(&R1);
	clearMat(&R2);
	clearMat(&t1);
	clearMat(&t2);
}

void FComposer::init(const FParams& params) {

	K1 = getDiagMat(params.f1, params.f1, 1);
	K2 = getDiagMat(params.f2, params.f2, 1);
	rz1 = getZR(params.ztheta1);
	rz2 = getZR(params.ztheta2);
	ry1 = getYR(params.ytheta1);
	ry2 = getYR(params.ytheta2);
	rx1 = getXR(params.xtheta1);

	estimateMatrices();
}

void FComposer::estimateMatrices() {
	CvMat* invR1 = matMul(rx1, ry1, rz1);
	CvMat* invR2 = matMul(ry2, rz2);
	CvMat* invK1 = inv(K1);
	CvMat* M1 = matMul(invR1, invK1);
	CvMat* invK2 = inv(K2);
	CvMat* M2 = matMul(invR2, invK2);
	CvMat* M2t = transpose(M2);
	CvMat* Fs = cvCreateMat(3, 3, CV_64FC1);
	cvZero(Fs);
	cvmSet(Fs, 1, 2, -1);
	cvmSet(Fs, 2, 1, 1);
	
	F = matMul(M2t, Fs, M1);

	R1 = inv(invR1);
	R2 = inv(invR2);

	H1 = matMul(K1, invR1, invK1);
	H2 = matMul(K1, invR2, invK2);

	t1 = getZero(3, 1);

	t2 = getZero(3, 1);
	cvmSet(t2, 0, 0, 1);

	clearMats(&invR1, &invR2, &invK1, &M1, &invK2, &M2, &M2t, &Fs);
}

FParams decomposeF(const CvMat* F) {
	//compute epipoles
	CvMat* e1 = null(F);
	CvMat* e2 = leftNull(F);

	//compute rz
	double ztheta1 = getZThetaToXAxis(e1);
	double ztheta2 = getZThetaToXAxis(e2);

	CvMat* rz1 = getZR(ztheta1);
	CvMat* rz2 = getZR(ztheta2);

	CvMat* ez1 = matMul(rz1, e1);
	CvMat* ez2 = matMul(rz2, e2);

	double d1 = cvmGet(ez1, 0, 0);
	double c1 = cvmGet(ez1, 2, 0);
	double d2 = cvmGet(ez2, 0, 0);
	double c2 = cvmGet(ez2, 2, 0);

	//compute a, b, c, d
	CvMat* Fz = transF(F, rz1, rz2);
	double t1[] = {1 / c1, 0,   0,
					 0,    1,   0,
					 0,    0, -1/d1};
	CvMat T1 = cvMat(3, 3, CV_64FC1, t1);
	double t2[] = {1 / c2, 0,   0,
					 0,    1,   0,
					 0,    0, -1/d2};
	CvMat T2 = cvMat(3, 3, CV_64FC1, t2);
	CvMat* Fbar = matMul(&T2, Fz, &T1);
	//cout<<"Fbar: "<<Fbar<<endl;

	double a = cvmGet(Fbar, 0, 0);
	double b = cvmGet(Fbar, 0, 1);
	double c = cvmGet(Fbar, 1, 0);
	double d = cvmGet(Fbar, 1, 1);

	/// release mats
	clearMats(&e1, &e2, &rz1, &rz2, &ez1, &ez2, &Fz, &Fbar);

	//compute f1, f2, ry1, ry2, rx
	double f1s = -a * c * d1 * d1 / (b * d + a * c * c1 * c1);
	if (f1s < 0) {
		cout<<"f1 square is less than zero! no solution"<<endl;
		throw "f1 square is less than zero! no solution";
	}
	double f1 = sqrt(f1s);

	double f2s = -a * b * d2 * d2 / (c * d + a * b * c2 * c2);
	if (f2s < 0) {
		cout<<"f2 square is less than zero! no solution"<<endl;
		throw "f2 square is less than zero! no solution";
	}
	double f2 = sqrt(f2s);

	double ytheta1 = getTheta(f1 * c1, d1);
	double ytheta2 = getTheta(f2 * c2, d2);

	double xtheta1 = getTheta(sqrt(-a * d / (b * c)), 1);

	FParams fp;
	fp.f1 = f1;
	fp.f2 = f2;
	fp.xtheta1 = xtheta1;
	fp.ytheta1 = ytheta1;
	fp.ytheta2 = ytheta2;
	fp.ztheta1 = ztheta1;
	fp.ztheta2 = ztheta2;

	return fp;
}

}