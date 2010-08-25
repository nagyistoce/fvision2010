#include "OffExporter.h"

#include "SurfaceMesh.h"

#include <cv.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace fvision { 

OffExporter::OffExporter(void)
{
}

OffExporter::~OffExporter(void)
{
}

void OffExporter::output(const SurfaceMesh& mesh, const std::string& filename) {
	ofstream ofs(filename.c_str());
	ofs<<"OFF"<<endl;
	ofs<<mesh.p3ds.size()<<" "<<mesh.polygons.size()<<" 0"<<endl;
	for (size_t i = 0; i < mesh.p3ds.size(); i++) {
		ofs<<mesh.p3ds[i].x<<" "<<mesh.p3ds[i].y<<" "<<mesh.p3ds[i].z<<endl;
	}
	for (size_t i = 0; i < mesh.polygons.size(); i++) {
		ofs<<mesh.polygons[i].size()<<" ";
		for (size_t j = 0; j < mesh.polygons[i].size(); j++) {
			ofs<<mesh.polygons[i][j]<<" ";
		}
		ofs<<endl;
	}
	ofs.close();
}

}