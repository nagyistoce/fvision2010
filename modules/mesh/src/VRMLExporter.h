#ifndef FVISION_VRML_EXPORTER_H_
#define FVISION_VRML_EXPORTER_H_

#include <cv.h>

#include <vector>
#include <iostream>

namespace fvision { 

class SurfaceMesh;

class VRMLExporter {
public:
	VRMLExporter(void);
public:
	~VRMLExporter(void);

public:
	void addShape(SurfaceMesh& mesh, const char* imageurl, CvSize imageSize);

	void output(const std::string& filename);
	
private:
	void addAppearance(std::ostream& os, const char* imageurl);

	void addIndexedFaceSet(std::ostream& os, SurfaceMesh& mesh, CvSize imageSize);

private:
	std::vector<std::string> shapes;
};

}
#endif // FVISION_VRML_EXPORTER_H_