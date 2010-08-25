#include ".\surfacemesh.h"

#include <fvision/geom.h>

#include <algorithm>

using namespace std;

namespace fvision { 

SurfaceMesh::SurfaceMesh(void)
{
}

SurfaceMesh::~SurfaceMesh(void)
{
}

void SurfaceMesh::transform(CvMat* R, double scale) {
	CvMat* RS = cvCloneMat(R);
	cvScale(R, RS, scale);
	modifyTransformPoint3DVector(RS, p3ds);
}

void SurfaceMesh::projectTexture(CvMat* M) {
	projectPoint3DVectorWithMatrix33(M, p3ds, p2ds);
}

void SurfaceMesh::drawOnImage(IplImage* src, CvScalar color) {
	for (size_t i = 0; i < polygons.size(); i++) {
		std::vector<int> polygon = polygons[i];
		for (size_t j = 0; j < polygon.size() - 1; j++) {
			cvLine(src, cvPointFrom32f(p2ds[polygon[j]]), cvPointFrom32f(p2ds[polygon[j+1]]), color);
		}
		cvLine(src, cvPointFrom32f(p2ds[polygon[polygon.size() - 1]]), cvPointFrom32f(p2ds[polygon[0]]), color);
	}
}

void SurfaceMesh::addMesh(SurfaceMesh& mesh) {
	int osize = (int)std::max(p3ds.size(), p2ds.size());
	p3ds.insert(p3ds.end(), mesh.p3ds.begin(), mesh.p3ds.end());
	p2ds.insert(p2ds.end(), mesh.p2ds.begin(), mesh.p2ds.end());
	int psize = (int)polygons.size();
	polygons.insert(polygons.end(), mesh.polygons.begin(), mesh.polygons.end());
	for (size_t i = psize; i < polygons.size(); i++) {
		for (size_t j = 0; j < polygons[i].size(); j++) {
			polygons[i][j] += osize;
		}
	}
}

bool isSame(const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
    return (fabs(p1.x - p2.x) < 0.0001 && fabs(p1.y - p2.y) < 0.0001);
}

int inSet(const CvPoint2D32f& p, const vector<CvPoint2D32f>& set) {
	for (size_t i = 0; i < set.size(); i++) {
		if (isSame(p, set[i])) return (int)i;
	}
	return -1;
}

//from p2ds
void SurfaceMesh::removeDuplicatePoints() {
	vector<CvPoint2D32f> noDuplicatedSet;
	vector<CvPoint3D64f> noDuplicatedSet3D;
	vector<int> indexMap;
	for (size_t i = 0; i < p2ds.size(); i++) {
		int index = inSet(p2ds[i], noDuplicatedSet);
		if (index == -1) { //not in set
			indexMap.push_back((int)(noDuplicatedSet.size()));
			noDuplicatedSet.push_back(p2ds[i]);
			noDuplicatedSet3D.push_back(p3ds[i]);
		} else { //duplicate
			indexMap.push_back(index);
		}
	}

	p2ds = noDuplicatedSet;
	p3ds = noDuplicatedSet3D;

	for (size_t i = 0; i < polygons.size(); i++) {
		for (size_t j = 0; j < polygons[i].size(); j++) {
			polygons[i][j] = indexMap[polygons[i][j]];
		}
	}
}


}
