#ifndef FVISION_SURFACE_MESH_H_
#define FVISION_SURFACE_MESH_H_

#include <vector>
#include <cv.h>

namespace fvision { 

class SurfaceMesh {
public:
	SurfaceMesh(void);
	~SurfaceMesh(void);

public:
	void addMesh(SurfaceMesh& mesh);

	void transform(CvMat* R, double scale);

	void projectTexture(CvMat* M);

	void drawOnImage(IplImage* src, CvScalar color = CV_RGB(0, 0, 255));

	void removeDuplicatePoints();

public:
	std::vector<CvPoint3D64f> p3ds;
	std::vector<CvPoint2D32f> p2ds;
	std::vector< std::vector<int> > polygons;
};


}
#endif // FVISION_SURFACE_MESH_H_