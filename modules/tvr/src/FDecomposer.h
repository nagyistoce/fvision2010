#pragma once

#include "FComposer.h"

#include <string>

namespace fvision {

class FDecomposer {
public:
	FDecomposer(void);
public:
	~FDecomposer(void);

public:
	void setFRange(double fmin, double fmax) {
		this->fmin = fmin;
		this->fmax = fmax;
	}

public:
	enum STATUS {
		SUCCESS,
		FAILURE,
		DEGENERATED
	};

public:
	FParams compute(const CvMat* F);

	STATUS getStatus() { return status;}

	std::string getStatusMessage();

public:
	FParams compute2(const CvMat* F);

private:
	void computeFz();
	void computeAbcd();
	void clear();

private:
	//range of f
	double fmin;
	double fmax;

public:
	CvMat* e1;
	CvMat* e2;
	CvMat* rz1;
	CvMat* rz2;
	CvMat* ez1;
	CvMat* ez2;

	double c1;
	double d1;
	double c2;
	double d2;

	double a;
	double b;
	double c;
	double d;

	CvMat* F;
	CvMat* Fz;

	FParams fp;

	STATUS status;

	bool isTanThetaY1SqaureNegative;
	bool isTanThetaY2SqaureNegative;

	bool isSinThetaXZero;
	bool isCosThetaXZero;

	bool isCosThetaY1Zero;
	bool isCosThetaY2Zero;
};

}