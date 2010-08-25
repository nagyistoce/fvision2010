#ifndef FVISION_MESH_UTILS_H_
#define FVISION_MESH_UTILS_H_

#include <cv.h>
#include <vector>

namespace fvision {

void computeDelaunayTriangles(std::vector<CvPoint2D32f> xs, std::vector<std::vector<int> >& polygons);

}

#endif // FVISION_MESH_UTILS_H_
