#ifndef FVISION_MEMDB_ALGORITHM_H_
#define FVISION_MEMDB_ALGORITHM_H_

#include "IndexedObjectsDatabase.h"
#include "MultipleFrameIndexedObjectsDatabase.h"

namespace fvision {

template <typename TDB>
double integrateDatabase(const TDB* db) {
	double sum = 0;
    typename TDB::ConstItemIterator* iter = db->getItemIterator();
	while (iter->hasNext()) {
        const typename TDB::Item* item = iter->next();
		sum += (double)item->object;
	}
	return sum;
}

template <typename TDB>
double computeDatabaseAverage(const TDB* db) {
	if (db->size() == 0) return 0;
	double sum = integrateDatabase(db);
	return sum / db->size();
}

}
#endif // FVISION_MEMDB_ALGORITHM_H_
