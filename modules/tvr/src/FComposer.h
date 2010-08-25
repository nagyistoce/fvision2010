#pragma once

#include <cv.h>

#include <iostream>

namespace fvision {

/*!
Using 7 parameters to compose a F

Assuming K = diag(f, f, 1)
F can be decomposed to seven parameters: 
f1:  focal length of the first camera
f2:  focal length of the second camera
rz1: rotation around z axis of the first camera
rz2: rotation around z axis of the second camera
ry1: rotation around y axis of the first camera
ry2: rotation around y axis of the second camera
rx1:  rotation around x axis of the frist camera

F  = R1zt * invK2t * R2yt * Fs * Rx1 * R1y * invK1 * R1z
Fs = [0  0  0; 
0  0 -1; 
0  1  0]

*/
class FParams {
public:
	FParams() {
		f1 = 1;
		f2 = 1;
		ztheta1 = 0;
		ztheta2 = 0;
		ytheta1 = 0;
		ytheta2 = 0;
		xtheta1 = 0;
	}

public:
	double f1;
	double f2;
	double ztheta1;
	double ztheta2;
	double ytheta1;
	double ytheta2;
	double xtheta1;
	//double xtheta2 = 0
};

std::ostream& operator<<(std::ostream& os, const FParams& fp);
std::istream& operator>>(std::istream& is, FParams& fp);

/*!
based on seven parameters, construct a series of matrices
fundamental matrix: F = R1zt * invK2t * R2yt * Fs * Rx1 * R1y * invK1 * R1z
camera matrices: K, R, t; Rx|Ry|Rz
homography for stereo rectification: H1, H2

K = diag(f, f, 1)
*/
class FComposer {
public:
	FComposer();

	FComposer(const FParams& params);

	FComposer(const FComposer& fc);
	FComposer& operator=(const FComposer& rhs);

	~FComposer();

public:
	FParams getFParams() const { return params; }
/*
	CvMat* getF() const { return F; }
	CvMat* getH1() const { return H1; }
	CvMat* getH2() const { return H2; }
	CvMat* getK1() const { return K1; }
	CvMat* getK2() const { return K2; }
	CvMat* getR1() const { return R1; }
	CvMat* getR2() const { return R2; }
*/
private:
	void clearData();

	void init(const FParams& params);
	void estimateMatrices();

private:
	FParams params;

public:
	CvMat* K1;
	CvMat* K2;
	CvMat* rz1;
	CvMat* rz2;
	CvMat* ry1;
	CvMat* ry2;
	CvMat* rx1;

	//CvMat* Fs;

	CvMat* F;

	CvMat* H1;
	CvMat* H2;

	CvMat* R1;
	CvMat* R2;

	CvMat* t1;
	CvMat* t2;
};

/** decompose F to seven parameters
 *
 * need to be improved to handle exceptions
 *
 * @param F the fundamental matrix
 * @exception const char* when decomposition fails
 * @return A FParams object which contains 7 parameters
 *
 * @see FParams FComposer
 */
FParams decomposeF(const CvMat* F);

}