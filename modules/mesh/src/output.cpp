#include "output.h"

#include "MeshCreator.h"
#include "OffExporter.h"
#include "VRMLExporter.h"

#include "highgui.h"

#include <fstream>

using namespace std;

namespace fvision { 

void outputOff(const std::string& filename, 
			   const vector<CvPoint3D64f>& Xs, 
			   const vector<CvPoint2D32f>& xs, 
			   double maxDepth) 
{
	MeshCreator mc(maxDepth);
	SurfaceMesh mesh = mc.create(Xs, xs);

	OffExporter offExporter;
	offExporter.output(mesh, filename);
}

void outputVRML(const std::string& filename, 
				const vector<CvPoint3D64f>& Xs, 
				const vector<CvPoint2D32f>& xs, 
				const char* imagePath, 
				double maxDepth) 
{

	MeshCreator mc(maxDepth);
	SurfaceMesh mesh = mc.create(Xs, xs);

	IplImage* src = cvLoadImage(imagePath);

	VRMLExporter vrmlExporter;
	vrmlExporter.addShape(mesh, imagePath, cvGetSize(src));
	vrmlExporter.output(filename);

	cvReleaseImage(&src);
}

}