#pragma once

#include <cv.h>
#include <vector>

/*!
 * reconstruction of cameras and structure
 * estimate point's 3d position from two cameras and point correspondences
 */
namespace fvision {

typedef CvPoint3D64f Point3D;

/**given two caemra matrices and the corresponding image points, estiamte the point's 3D position based on triagulation
 * See <<Multiple View Geometry>> p312
 */
CvMat* triangulate(const CvMat* P1, const CvMat* P2, const CvPoint2D32f& x1, const CvPoint2D32f& x2);

Point3D triangulateToPoint3D(const CvMat* P1, 
							 const CvMat* P2, 
							 const CvPoint2D32f& x1, 
							 const CvPoint2D32f& x2
							 );

std::vector<Point3D> triangulateToPoint3Ds(const CvMat* P1, 
										   const CvMat* P2, 
										   const std::vector<CvPoint2D32f>& x1s, 
										   const std::vector<CvPoint2D32f>& x2s);

/** compute reprojection error
 * (distance(P1 * X, x1) + distance(P2 * X, x2)) / 2
 */
double triangulationError(const Point3D& X, const CvMat* P1, const CvMat* P2, const CvPoint2D32f& x1, const CvPoint2D32f& x2, int metric = 1);

/** compute reprojection error
 * distance(P * X, x)
 */
double getReprojectionError(const Point3D& X, const CvMat* P, const CvPoint2D32f& x, int metric = 1);

/** compute reprojection errors
* distance(P * X, x)
*/
std::vector<double> getReprojectionErrors(const Point3D& X, const std::vector<CvMat*>& Ps, const std::vector<CvPoint2D32f>& xs, int metric = 1);

/** compute average reprojection error
 * sum(distance(Pi * X, xi)) / n
 */
double getAverageReprojectionError(const Point3D& X, const std::vector<CvMat*>& Ps, const std::vector<CvPoint2D32f>& xs, int metric = 1);

/** compute reprojection errors
 *
 */
std::vector<double> triangulationErrors(const std::vector<Point3D>& Xs, const CvMat* P1, const CvMat* P2, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s, int metric = 1);

//given essential matrix, normalized point correspondences
//compute camera external matrices
//see Multiple View Geometry P257 ~ P259
void computeRt(const CvMat* E, const std::vector<CvPoint2D32f>& nx1s, const std::vector<CvPoint2D32f>& nx2s, CvMat* R, CvMat* t);

//compute how many points are in front of both cameras, defined by [I|0] and [R|t]
//there are four solutions of cameras based on essential matrix, only one is valid, 
//and need point correspondences to find the valid one
int computeValidPointsNum(CvMat* R, CvMat* t, const std::vector<CvPoint2D32f>& nx1s, const std::vector<CvPoint2D32f>& nx2s);

//return true if the reconstructed 3d point is in front of both cameras
bool isValid(CvMat* P1, CvMat* P2, const CvPoint2D32f& x1, const CvPoint2D32f& x2);

//proj_x1s = P1 * Xs, proj_x2s = P2 * Xs
//error = sum_error(proj_x1s, x1s) + sum_error(proj_x2s, x2s);
double computeReprojectionError(CvMat* P1, CvMat* P2, const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s, const std::vector<Point3D>& Xs);

/** triangulate a 3D point with cameras and projections using DLT algorithm
 * See <<Multiple View Geometry>> p312
 * P * X = x  -> x x PX = 0
 * x * (p3t * X) - (p1t * X) = 0
 * y * (p3t * X) - (p2t * X) = 0
 * 2 constraints:
 * x * p3t - p1t
 * y * p3t - p2t
 * weighted DLT
 * weight = 1 / p3t * X.  ->  1/depth
 */
CvMat* batchTriangulate(const std::vector<CvMat*>& Ps, const std::vector<CvPoint2D32f> xs, const CvMat* initmX);

Point3D batchTriangulate(const std::vector<CvMat*>& Ps, const std::vector<CvPoint2D32f> xs, const Point3D& initX);

Point3D batchTriangulate(const std::vector<CvMat*>& Ps, const std::vector<CvPoint2D32f> xs);

}