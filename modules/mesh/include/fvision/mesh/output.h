#ifndef FVISION_MESH_OUTPUT_H_
#define FVISION_MESH_OUTPUT_H_

/** output utils
 * Given 3d and 2d point cloud, output off file
 * Given 3d, 2d point cloud and image, output vrml
 *
 */

#include <cv.h>

#include <vector>
#include <string>

namespace fvision { 

void outputOff(const std::string& filename, 
			   const std::vector<CvPoint3D64f>& Xs, 
			   const std::vector<CvPoint2D32f>& xs, 
			   double maxDepth = 40.0);

void outputVRML(const std::string& filename, 
				const std::vector<CvPoint3D64f>& Xs, 
				const std::vector<CvPoint2D32f>& xs, 
				const char* imagePath, 
				double maxDepth = 40.0);

}
#endif // FVISION_MESH_OUTPUT_H_