#include "mesh_utils.h"

#include "Delaunay.h"

namespace fvision {

vertexSet getVertexSet(const vector<CvPoint2D32f>& xs) {
	vertexSet vset;
	for (size_t i = 0; i < xs.size(); i++) {
		vset.insert(vertex(xs[i].x, xs[i].y));
	}
	return vset;
}

int getIndex(const vertex* v, const vector<CvPoint2D32f>& xs) {
	for (size_t i = 0; i < xs.size(); i++) {
        if (fabs(xs[i].x - v->GetX()) < 0.0001 && fabs(xs[i].y - v->GetY()) < 0.0001) return (int)i;
	}
	return 0;
}

void computeDelaunayTriangles(std::vector<CvPoint2D32f> xs, std::vector<std::vector<int> >& polygons) {

	polygons.clear();

	vertexSet vset = getVertexSet(xs);
	triangleSet tset;

	Delaunay d;
	d.Triangulate(vset, tset);

	for (ctIterator it = tset.begin(); it != tset.end(); ++it) {
		vector<int> polygon;
		for (int i = 0; i < 3; i++) {
			int index = getIndex(it->GetVertex(i), xs);
			polygon.push_back(index);
			//cout<<"index: "<<index<<endl;
		}
		polygons.push_back(polygon);
	}

}

}
