#pragma once

#include <cv.h>

#include <vector>

/*!
two view geometry utils
F
P
H

*/
namespace fvision {

//F = e'xP'P+
CvMat* calcF(CvMat* P1, CvMat* P2);

//suppose x1' = H1 * x1, x2' = H2 * x2
//and suppose F is the fundamental matrix for {x1i, x2i} such that x2it * F * x1i = 0
//then F' = inv(transpose(H2)) * F * inv(H1) is the fundamental matrix for {x1i', x2i'} such that x2it' *  F' * x1i' = 0
CvMat* transF(const CvMat* F, const CvMat* H1, const CvMat* H2);

//usually, the image coordinates is differnt from camera plane coordinates.
//the difference is that y' = h - 1 - y. h is the window height
//Therefore, there is a transformation: H = [1, 0, 0; 0, -1, h - 1; 0, 0, 1]
CvMat* transFWithFlipY(const CvMat* F, double h);

//x' = Hx
//H = [scale, 0, 0; 0, scale, 0; 0, 0, 1]
CvMat* transFWithScale(const CvMat* F, double scale);

//suppose x = MH x' which may mean transform an image to another kind of image such as scale or enlarge operation
//and suppose H is the homograhpy which make a transformation for an image such that xt = H x.
//then H' = MH-1 H MH is the homography for {x'} such that xt' = H' x'. 
CvMat* transH(const CvMat* H, const CvMat* MH);

//h is the window height
CvMat* transHWithFlipY(const CvMat* H, double h);

//H = [1/scale, 0, 0; 0, 1/scale, 0; 0, 0, 1]
CvMat* transHWithScale(const CvMat* H, double scale);

//H = [1, 0, 0; 0, -1, h - 1; 0, 0, 1]
CvMat* getHWithFlipY(double h);

//H = [scale, 0, 0; 0, scale, 0; 0, 0, 1]
CvMat* getHWithScale(double scale);

CvMat* getHWithRotate(const CvMat* K, double theta);

//x' = x + xv, y' = y + yv
//H = [1, 0, xv; 0, 1, yv; 0, 0, 1]
CvMat* getHWithTranslation(double xv, double yv);
CvMat* getHWithTranslation(const CvPoint2D32f& p);

//F = [e2]xM
CvMat* getFM(const CvMat* F, double a = 0.1);


//T 3x4 matrix.
CvPoint3D32f transformPoint3DWithMatrix34(const CvMat* T, const CvPoint3D32f& p);

std::vector<CvPoint3D32f> transformPoint3DsWithMatrix34(const CvMat* T, const std::vector<CvPoint3D32f>& xs);

//x2 - Hx1
double computeHomographyDistance(const CvMat* H, const CvPoint2D32f& x1, const CvPoint2D32f& x2);

//H = K2*(R - t*Nt/d)*invK1
CvMat* estimateNtd(const CvMat* K1, const CvMat* K2, const CvMat* R, const CvMat* t, const CvMat* H);

//when we assume ground is lower than camera center
//in coordinates system invK x
//a point (x0, y0), if we assume it is on ground plane, and we assume the ground plane has the y coordinates of -1
//then the 3D coordinates of the point is (-x0/y0, -1, -1/y0)
//in ground plane, we want the coordinates to be (-x0/y0, -1/y0, 1) = (x0, 1, -y0), which is equal to T x (x0, y0, 1)
//[1, 0, 0;
//  0, 0, 1;
//  0, -1, 0]
CvMat* getT();

//get rotation parameters of epipole
//ez = Rz * e
//Ry * invK * ez = (1, 0, 0) = e0
//e = invRz * K * invRy * e0
void getEpipoleThetas(CvMat* e, CvMat* K, double& ztheta, double& ytheta);

//E = transpose(K2) * F * K1
CvMat* estimateEFromFK(const CvMat* F, const CvMat* K1, const CvMat* K2);
CvMat* getFFromEK(const CvMat* E, const CvMat* K1, const CvMat* K2);
//first E = = transpose(K2) * F * K1
//second E' = UDV'where D is normalized such that first two single values are set to be equal
//see <<multiple view geometry>> pp 294
CvMat* getEFromFK(const CvMat* F, const CvMat* K1, const CvMat* K2);


}
