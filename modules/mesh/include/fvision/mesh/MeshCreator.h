#ifndef FVISION_MESH_CREATOR_H_
#define FVISION_MESH_CREATOR_H_

#include "SurfaceMesh.h"

namespace fvision { 

class MeshCreator {
public:
	MeshCreator(double maxDepth = 40.0) {
		this->maxDepth = maxDepth;
	}

public:
	/** Given 3d and 2d point cloud, create a mesh based on Delaunay triangulation
	 *
	 */
	SurfaceMesh create(const std::vector<CvPoint3D64f>& Xs, const std::vector<CvPoint2D32f>& xs);

private:
	double maxDepth;

};

}
#endif // FVISION_MESH_CREATOR_H_