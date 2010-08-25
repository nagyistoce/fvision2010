#include "VRMLExporter.h"

#include "SurfaceMesh.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

namespace fvision { 

VRMLExporter::VRMLExporter(void)
{
}

VRMLExporter::~VRMLExporter(void)
{
}

void VRMLExporter::addShape(SurfaceMesh& mesh, const char* imageurl, CvSize imageSize) {
	ostringstream os;
	addAppearance(os, imageurl);
	addIndexedFaceSet(os, mesh, imageSize);
	shapes.push_back(os.str());
}

void VRMLExporter::output(const std::string& filename) {
	ofstream ofs(filename.c_str());
	ofs<<"#VRML V2.0 utf8"<<endl;
	for (int i = 0; i < (int)shapes.size(); i++) {
		ofs<<"Shape {"<<endl;
		ofs<<shapes[i]<<endl;
		ofs<<"}"<<endl;
	}
	ofs.close();
}

void VRMLExporter::addAppearance(ostream& os, const char* imageurl) {
	os << "appearance Appearance { \r\n\t texture ImageTexture { \r\n\t\t url \" ";
	os << imageurl;
	os << "\"\r\n }}";
}

void VRMLExporter::addIndexedFaceSet(ostream& os, SurfaceMesh& mesh, CvSize imageSize) {
	os << "\r\n geometry IndexedFaceSet { " <<endl;

	os << "\r\n\t ccw FALSE"<<endl;
	os << "\r\n\t solid FALSE"<<endl;

	os << "\r\n\t coord Coordinate { \r\n\t\t point [ ";
	for (int i = 0; i < (int)mesh.p3ds.size(); i++) {
		os << "\r\n\t\t\t";
		os << mesh.p3ds[i].x;
		os << " ";
		os << mesh.p3ds[i].y;
		os << " ";
		os << -mesh.p3ds[i].z; //in single view modeling system, the coordinates system is left hand
		os << ", ";
	}
	os << "\r\n\t\t ]}";

	os << "\r\n\t coordIndex [";
	for (int i = 0; i < (int)mesh.polygons.size(); i++) {
		os << "\r\n\t\t";
		for (int j = 0; j < (int)mesh.polygons[i].size(); j++) {
			os << mesh.polygons[i][j];
			os << ", ";
		}
		os << "-1, ";
	}
	os << "\r\n\t ]";

	os << "\r\n\t texCoord TextureCoordinate { \r\n\t\t point [ ";
	for (int i = 0; i < (int)mesh.p2ds.size(); i++) {
		os << "\r\n\t\t\t";
		os << mesh.p2ds[i].x / imageSize.width;
		os << " ";
		os << (1 - mesh.p2ds[i].y / imageSize.height);
		os << ", ";
	}
	os << "\r\n\t ]}";

	os << "\r\n }";
}

}