#ifndef FVISION_OFF_EXPORTER_H_
#define FVISION_OFF_EXPORTER_H_

#include <string>

namespace fvision { 

class SurfaceMesh;

/** export OFF file
 *
 */
class OffExporter {
public:
	OffExporter(void);
public:
	~OffExporter(void);

public:
	void output(const SurfaceMesh& mesh, const std::string& filename);
};

}
#endif // FVISION_OFF_EXPORTER_H_