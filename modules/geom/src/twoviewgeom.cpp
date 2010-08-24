#include "twoviewgeom.h"

#include "rotation.h"
#include <point_transform_template.h>

#include <fvision/cvutils.h>

#include <iostream>

using namespace std;

namespace fvision {

//F = e'xP'P+
CvMat* calcF(CvMat* P1, CvMat* P2) {
	
	CvMat* C = null(P1);
	//cout<<"C "<<matString(C)<<endl;
	//cout<<"P1 C: "<<matString(matMul(P1, C))<<endl;
	
	CvMat* e2 = matMul(P2, C);
	//cout<<"e2: "<<matString(e2)<<endl;
	
	CvMat* e2x = getCrossMatrix(e2);
	CvMat* P1P = pseudoinverse(P1);

	CvMat* F = matMul(e2x, P2, P1P);

	cvReleaseMat(&C);
	cvReleaseMat(&e2);
	cvReleaseMat(&e2x);

	return F;
}

CvMat* transF(const CvMat* F, const CvMat* H1, const CvMat* H2) 
{
	CvMat* H2t = transpose(H2);
	CvMat* invH2t = inv(H2t);
	CvMat* invH1 = inv(H1);
	CvMat* nF = matMul(invH2t, F, invH1);
	clearMats(&H2t, &invH2t, &invH1);
	return nF;
}

CvMat* transFWithFlipY(const CvMat* F, double h) 
{
	double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
	CvMat H = cvMat(3, 3, CV_64FC1, mh);
	return transF(F, &H, &H);
}

CvMat* transFWithScale(const CvMat* F, double scale) 
{
	double mh[] = {scale, 0, 0, 0, scale, 0, 0, 0, 1};
	CvMat H = cvMat(3, 3, CV_64FC1, mh);
	return transF(F, &H, &H);
}

CvMat* transH(const CvMat* H, const CvMat* MH) {
	CvMat* inv_MH = cvCreateMat(3, 3, CV_64FC1);
	cvInvert(MH, inv_MH);
	CvMat* nH = matMul(inv_MH, H, MH);
	cvReleaseMat(&inv_MH);
	return nH;
}

CvMat* transHWithFlipY(const CvMat* H, double h) {
	double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
	CvMat MH = cvMat(3, 3, CV_64FC1, mh);
	return transH(H, &MH);
}

CvMat* transHWithScale(const CvMat* H, double scale) {
	double mh[] = {1/scale, 0, 0, 0, 1/scale, 0, 0, 0, 1};
	CvMat MH = cvMat(3, 3, CV_64FC1, mh);
	return transH(H, &MH);
}

//H = [1, 0, 0; 0, -1, h - 1; 0, 0, 1]
CvMat* getHWithFlipY(double h) {
	CvMat* H = cvCreateMat(3, 3, CV_64FC1);
	double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
	cvSetData(H, mh, CV_AUTOSTEP);
	return H;
}

//H = [scale, 0, 0; 0, scale, 0; 0, 0, 1]
CvMat* getHWithScale(double scale) {
	CvMat* H = cvCreateMat(3, 3, CV_64FC1);
	double mh[] = {scale, 0, 0, 0, scale, 0, 0, 0, 1};
	cvSetData(H, mh, CV_AUTOSTEP);
	return cloneMat(H);
}

CvMat* getHWithRotate(const CvMat* K, double theta) {

	//float theta = 5 * 3.1415 / 180;
	double tanTheta = tan(theta);
	double mh[] = {1, 0, tanTheta,  0, 1, 0,  -tanTheta, 0, 1};
	CvMat MH = cvMat(3, 3, CV_64FC1, mh);

	CvMat* invK = inv(K);
	CvMat* H = matMul(K, &MH, inv(K));
	
	cvReleaseMat(&invK);
	return H;
}

CvMat* getHWithTranslation(double xv, double yv) {
	CvMat* H = cvCreateMat(3, 3, CV_64FC1);
	double mh[] = {1, 0, xv, 0, 1, yv, 0, 0, 1};
	cvSetData(H, mh, CV_AUTOSTEP);
	return cloneMat(H);
}

CvMat* getHWithTranslation(const CvPoint2D32f& p) {
	return getHWithTranslation(p.x, p.y);
}

CvMat* getFM(const CvMat* F, double a) {
	CvMat* e2 = leftNull(F);
	CvMat* e2x = getCrossMatrix(e2);
	CvMat* e2xF = matMul(e2x, F);

	double v[] = {a, 0, 0};
	CvMat V = cvMat(3, 1, CV_64FC1, v);
	CvMat* e2Vt = matMul(e2, transpose(&V));

	CvMat* M = cvCreateMat(3, 3, CV_64FC1);
	cvAdd(e2xF, e2Vt, M);

	//==DEBUG
	cout<<"F: "<<F<<endl;
	cout<<"e2xF: "<<e2xF<<endl;
	cout<<"e2xM: "<<matMul(e2x, M)<<endl;
	cout<<"M: "<<M<<endl;
	cout<<"det(e2xF): "<<cvDet(e2xF)<<endl;
	cout<<"det(M): "<<cvDet(M)<<endl;
	//==

	cvReleaseMat(&e2);
	cvReleaseMat(&e2x);
	cvReleaseMat(&e2xF);
	cvReleaseMat(&e2Vt);

	return M;
}

CvPoint3D32f transformPoint3DWithMatrix34(const CvMat* T, const CvPoint3D32f& p) {
	CvMat* mp = hmatFromPoint3D(p);
	CvMat* tmp = matMul(T, mp);
	CvPoint3D32f tp;
	matToPoint3D(tmp, tp);
	cvReleaseMat(&mp);
	cvReleaseMat(&tmp);
	return tp;
}

vector<CvPoint3D32f> transformPoint3DsWithMatrix34(const CvMat* T, const vector<CvPoint3D32f>& xs) {
	vector<CvPoint3D32f> nxs;
	for (int i = 0; i < (int)xs.size(); i++) {
		nxs.push_back(transformPoint3DWithMatrix34(T, xs[i]));
	}
	return nxs;
}

double computeHomographyDistance(const CvMat* H, const CvPoint2D32f& x1, const CvPoint2D32f& x2) {
	CvPoint2D32f hx1 = transformPoint(H, x1);
	return pointDistance(hx1, x2);
}

CvMat* estimateEFromFK(const CvMat* F, const CvMat* K1, const CvMat* K2) {
	CvMat* K2T = transpose(K2);
	CvMat* E = matMul(K2T, F, K1);
	cvReleaseMat(&K2T);
	return E;
}

CvMat* getEFromFK(const CvMat* F, const CvMat* K1, const CvMat* K2) {
	CvMat* K2T = transpose(K2);
	CvMat* E = matMul(K2T, F, K1);
	CvMat* U;
	CvMat* V;
	CvMat* D;
	svd(E, &D, &U, &V); //E = U*D*V'
	cout<<"s0 "<<cvmGet(D, 0, 0)<<endl;
	cout<<"s1 "<<cvmGet(D, 1, 1)<<endl;
	cout<<"s_ratio: "<<cvmGet(D, 1, 1) / cvmGet(D, 0, 0)<<endl;
	double m = (cvmGet(D, 0, 0) + cvmGet(D, 1, 1)) / 2;
	cvmSet(D, 0, 0, m);
	cvmSet(D, 1, 1, m);
	cvmSet(D, 2, 2, 0);
	CvMat* tV = transpose(V);
	CvMat* tE = matMul(U, D, tV);

	cvReleaseMat(&K2T);
	cvReleaseMat(&E);
	cvReleaseMat(&U);
	cvReleaseMat(&V);
	cvReleaseMat(&D);
	cvReleaseMat(&tV);

	return tE;
}


//dstCol = s * srcCol
//s = (s0 * d0 + s1 * d1 + s2 * d2) / (s0 * s0 + s1 * s1 + s2 * s2)
double estimateScale(const CvMat* srcCol, const CvMat* dstCol) {
	double s0 = cvmGet(srcCol, 0, 0);
	double s1 = cvmGet(srcCol, 1, 0);
	double s2 = cvmGet(srcCol, 2, 0);

	double d0 = cvmGet(dstCol, 0, 0);
	double d1 = cvmGet(dstCol, 1, 0);
	double d2 = cvmGet(dstCol, 2, 0);

	return (s0 * d0 + s1 * d1 + s2 * d2) / (s0 * s0 + s1 * s1 + s2 * s2);
}

CvMat* estimateNtd(const CvMat* K1, const CvMat* K2, const CvMat* R, const CvMat* t, const CvMat* H) {

	CvMat* NH = matMul(inv(K2), H, K1);
	CvMat* RmNH = sub(R, NH);

	CvMat* NNH = scale(NH, 1/cvmGet(NH, 2, 2));
	CvMat* NR = scale(R, 1/cvmGet(R, 2, 2));
	CvMat* NRmNNH = sub(NR, NNH);

	cout<<"R: "<<R<<endl;
	cout<<"NH: "<<NH<<endl;
	cout<<"RmNH: "<<RmNH<<endl;

	cout<<"NR: "<<NR<<endl;
	cout<<"NNH: "<<NNH<<endl;
	cout<<"NRmNNH: "<<NRmNNH<<endl;

	CvMat* Ntd = cvCreateMat(1, 3, CV_64FC1);

	CvMat* c0 = getCol(NRmNNH, 0);
	CvMat* c1 = getCol(NRmNNH, 1);
	CvMat* c2 = getCol(NRmNNH, 2);

	cvmSet(Ntd, 0, 0, estimateScale(t, c0));
	cvmSet(Ntd, 0, 1, estimateScale(t, c1));
	cvmSet(Ntd, 0, 2, estimateScale(t, c2));

	cout<<"H: "<<H<<endl;
	cout<<"recovered H: "<<matMul(K2, sub(R, matMul(t, Ntd)), inv(K1))<<endl;

	cvReleaseMat(&NH);
	cvReleaseMat(&RmNH);

	cvReleaseMat(&c0);
	cvReleaseMat(&c1);
	cvReleaseMat(&c2);

	return Ntd;
}

CvMat* getT() {
	CvMat* T = cvCreateMat(3, 3, CV_64FC1);
	double th[] = {1, 0, 0,  0, 0, 1,  0, -1, 0};
	cvSetData(T, th, CV_AUTOSTEP);
	return cloneMat(T);
}

void getEpipoleThetas(CvMat* e, CvMat* K, double& ztheta, double& ytheta) {
	CvMat* invK = inv(K);
	CvMat* ne = matMul(invK, e);
	ztheta = getZThetaToXAxis(ne);
	CvMat* rz = getZR(ztheta);
	CvMat* rze = matMul(rz, e);
	CvMat* nrze = matMul(invK, rze);
	ytheta = getYThetaToInfinity(nrze);

	cvReleaseMat(&ne);
	cvReleaseMat(&rz);
	cvReleaseMat(&rze);
	cvReleaseMat(&invK);
	cvReleaseMat(&nrze);
}

//F = inv(transpose(K2)) * E * inv(K1);
CvMat* getFFromEK(const CvMat* E, const CvMat* K1, const CvMat* K2) {
	CvMat* invK1 = inv(K1);
	CvMat* K2t = transpose(K2);
	CvMat* invK2t = inv(K2t);
	CvMat* F = matMul(invK2t, E, invK1);

	cvReleaseMat(&invK1);
	cvReleaseMat(&K2t);
	cvReleaseMat(&invK2t);

	return F;
}


}