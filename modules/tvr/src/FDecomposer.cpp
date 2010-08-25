#include "FDecomposer.h"

#include <fvision/geom.h>

using namespace std;

namespace fvision {

FDecomposer::FDecomposer(void) {

	e1 = cvCreateMat(3, 1, CV_64FC1);
	e2 = cvCreateMat(3, 1, CV_64FC1);

	rz1 = cvCreateMat(3, 3, CV_64FC1);
	rz2 = cvCreateMat(3, 3, CV_64FC1);

	ez1 = cvCreateMat(3, 1, CV_64FC1);
	ez2 = cvCreateMat(3, 1, CV_64FC1);

	F = cvCreateMat(3, 3, CV_64FC1);

	Fz = NULL;

	status = SUCCESS;

	fmin = 100;
	fmax = 1500;
}

FDecomposer::~FDecomposer(void) {
}


FParams FDecomposer::compute2(const CvMat* F) {

	cvCopy(F, this->F);

	computeFz();

	double f11 = cvmGet(Fz, 0, 0);
	double f12 = cvmGet(Fz, 0, 1);
	double f21 = cvmGet(Fz, 1, 0);
	double f22 = cvmGet(Fz, 1, 1);

	double p1 = f11 * f22 / f12 / f21;
	double p2 = f12 * f22 / f11 / f21;
	double p3 = f21 * f22 / f11 / f12;

	if (p2 > -1) {
		status  = FAILURE;
		//fStatus = TAN_THETAY1_SQUARE_NEGTIVE;
		return fp;
	}

	if (p3 > -1) {
		status  = FAILURE;
		//fStatus = TAN_THETAY2_SQUARE_NEGTIVE;
		return fp;
	}

	//compute theta and f

	fp.xtheta1 = getTheta(sqrt(-p1), 1);

    double fabsTan1y = 1 / sqrt(-(p2+1));
    double fabsTan2y = 1 / sqrt(-(p3+1));

	if (d1/c1 >= 0) {
        fp.f1 = fabsTan1y * d1 / c1;
        fp.ytheta1 = getTheta(fabsTan1y, 1);
	} else {
        fp.f1 = -fabsTan1y * d1 / c1;
        fp.ytheta1 = getTheta(-fabsTan1y, 1);
	}

	if (d2 / c2 >= 0) {
        fp.f2 = fabsTan2y * d2 / c2;
        fp.ytheta2 = getTheta(fabsTan2y, 1);
	} else {
        fp.f2 = -fabsTan2y * d2 / c2;
        fp.ytheta2 = getTheta(-fabsTan2y, 1);
	}

	return fp;
}

void FDecomposer::computeFz() {

	//compute epipoles
	null(F, e1);
	leftNull(F, e2);
	//null(transpose(F), e2);

	//compute rz
	fp.ztheta1 = getZThetaToXAxis(e1);
	fp.ztheta2 = getZThetaToXAxis(e2);

	getZR(fp.ztheta1, rz1);
	getZR(fp.ztheta2, rz2);

	cvMatMul(rz1, e1, ez1);
	cvMatMul(rz2, e2, ez2);

	d1 = cvmGet(ez1, 0, 0);
	c1 = cvmGet(ez1, 2, 0);
	d2 = cvmGet(ez2, 0, 0);
	c2 = cvmGet(ez2, 2, 0);

	//cout<<"d1: "<<d1<<endl;
	//cout<<"c1: "<<c1<<endl;
	//cout<<"d2: "<<d2<<endl;
	//cout<<"c2: "<<c2<<endl;

	//compute Fz
	if (Fz != NULL) cvReleaseMat(&Fz);
	Fz = transF(F, rz1, rz2);

}

/**
 * <code>
 *   Fz = [a * c1 * c2,    b * c2,   -a * d1 * c2;
 *            c * c1,        d,        -c * d1;
 *        -a * c1 * d2,   -b * d2,    a * d1 * d2]
 * </code>
 */
void FDecomposer::computeAbcd() {

	d = cvmGet(Fz, 1, 1);

    if (fabs(c1) > fabs(d1)) {
		c = cvmGet(Fz, 1, 0) / c1;
        if (fabs(c2) > fabs(d2)) {
			a = cvmGet(Fz, 0, 0) / c1 / c2;
		} else {
			a = -cvmGet(Fz, 2, 0) / c1 / d2;
		}
	} else { //d1
		c = -cvmGet(Fz, 1, 2) / d1;
        if (fabs(c2) > fabs(d2)) { //d1, c2
			a = -cvmGet(Fz, 0, 2) / d1 / c2;
		} else { //d1, d2
			a = cvmGet(Fz, 2, 2) / d1 / d2;
		}
	}

    if (fabs(c2) > fabs(d2)) {
		b = cvmGet(Fz, 0, 1) / c2;
	} else {
		b = -cvmGet(Fz, 2, 1) / d2;
	}

	//cout<<"a = "<<a<<endl;
	//cout<<"b = "<<b<<endl;
	//cout<<"c = "<<c<<endl;
	//cout<<"d = "<<d<<endl;
}

void FDecomposer::clear() {

	status = SUCCESS;

	isTanThetaY1SqaureNegative = false;
	isTanThetaY2SqaureNegative = false;

	isSinThetaXZero = false;
	isCosThetaXZero = false;

	isCosThetaY1Zero = false;
	isCosThetaY2Zero = false;

}

FParams FDecomposer::compute(const CvMat* F) {

	clear();

	cvCopy(F, this->F);

	computeFz();

	computeAbcd();

	double ad = a * d;
	double bc = b * c;

    double sqrt_fabs_ad = sqrt(fabs(ad));
    double sqrt_fabs_bc = sqrt(fabs(bc));

    double xtheta_1 = getTheta(sqrt_fabs_ad, sqrt_fabs_bc);
    double xtheta_2 = getTheta(sqrt_fabs_ad, -sqrt_fabs_bc);
    if (fabs(xtheta_1) < 1e-7) { //degenerate, xtheta = 0
		fp.xtheta1 = 0;
		status = DEGENERATED;
		isSinThetaXZero = true;
		//set f1 and compute f2
		fp.f1 = fmax / 3;
		fp.f2 = sqrt(d2 * d2 /(c * c * (c1 * c1 + d1 * d1 / fp.f1 / fp.f1) / b / b - c2  * c2));
    } else if (fabs(cos(xtheta_1)) < 1e-7) {
		fp.xtheta1 = xtheta_1;
		status = DEGENERATED;
		this->isCosThetaXZero = true;
		fp.f1 = fmax / 3;
		fp.f2 = sqrt( d2 * d2 / (d * d / (c1 * c1 + d1 * d1 / fp.f1 / fp.f1) / a / a - c2  * c2));
	} else {
		double f1s = -a * c * d1 * d1 / (b * d + a * c * c1 * c1);
		cout<<"f1s: "<<f1s<<endl;
		if (f1s < 0) {
			status  = FAILURE;
			isTanThetaY1SqaureNegative = true;
			fp.f1 = fmax / 3;
		}
		fp.f1 = sqrt(f1s);

		double f2s = -a * b * d2 * d2 / (c * d + a * b * c2 * c2);
		if (f2s < 0) {
			status  = FAILURE;
			isTanThetaY2SqaureNegative = true;
			fp.f2 = fmax / 3;
		}
		fp.f2 = sqrt(f2s);

		fp.ytheta1 = getTheta(fp.f1 * c1, d1);
		fp.ytheta2 = getTheta(fp.f2 * c2, d2);

        if (fabs(c1) > fabs(d1)) {
			if (a * sin(fp.ytheta1) / b / c1 > 0) fp.xtheta1 = xtheta_1;
			else fp.xtheta1 = xtheta_2;
		} else {
			if (a * d1 / b / cos(fp.ytheta1) > 0) fp.xtheta1 = xtheta_1;
			else fp.xtheta1 = xtheta_2;
		}
	}

	fp.ytheta1 = getTheta(fp.f1 * c1, d1);
	fp.ytheta2 = getTheta(fp.f2 * c2, d2);

	return fp;
}

string FDecomposer::getStatusMessage() {
	switch(status) {
		case SUCCESS: return "SUCCESS";
		case FAILURE: {
			return "FAILURE";
					  }
		case DEGENERATED: 
			{
				return "DEGENERATED";
			}
	}
	return "DONT KNOW";
}

}
