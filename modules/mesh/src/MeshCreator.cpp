#include "MeshCreator.h"

#include "mesh_utils.h"

using namespace std;

namespace fvision { 

SurfaceMesh MeshCreator::create(const vector<CvPoint3D64f>& Xs, const vector<CvPoint2D32f>& xs) {

	vector<CvPoint2D32f> ixs;
	vector<CvPoint3D64f> iXs;
	for (size_t i = 0; i < xs.size(); i++) {
		if (Xs[i].z < 0 || Xs[i].z > maxDepth) continue;
		ixs.push_back(xs[i]);
		iXs.push_back(Xs[i]);
	}

	SurfaceMesh mesh;
	mesh.p2ds = ixs;
	mesh.p3ds = iXs;

	computeDelaunayTriangles(ixs, mesh.polygons);

	return mesh;
}

}