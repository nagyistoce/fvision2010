#ifndef FVISION_MEMDB_UTILS_H_
#define FVISION_MEMDB_UTILS_H_

#include "IndexedObjectsDatabase.h"
#include "MultipleFrameIndexedObjectsDatabase.h"

#include <iostream>

namespace fvision {

namespace memdb_internal {
    template <typename T>
    std::vector<T> intersection(const std::set<T>& v1, const std::set<T>& v2) {
        std::vector<T> nv(v1.size() + v2.size());
        typename std::vector<T>::iterator iter = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), nv.begin());
        nv.resize(iter - nv.begin());
        return nv;
    }
    template <typename T>
    std::vector<T> intersection(const std::vector<T>& v1, const std::vector<T>& v2) {
        std::vector<T> sv1 = v1;
        sort(sv1.begin(), sv1.end());
        std::vector<T> sv2 = v2;
        sort(sv2.begin(), sv2.end());
        std::vector<T> nv(v1.size() + v2.size());
        typename std::vector<T>::iterator iter = set_intersection(sv1.begin(), sv1.end(), sv2.begin(), sv2.end(), nv.begin());
        nv.resize(iter - nv.begin());
        return nv;
    }
}

template<typename TObject>
std::vector<int> getCommonIds(const MultipleFrameIndexedObjectsDatabase<TObject>* db, int frameId1, int frameId2) {
	std::set<int> ids1 = db->getAllObjectIdsInFrame(frameId1);
	std::set<int> ids2 = db->getAllObjectIdsInFrame(frameId2);
    return memdb_internal::intersection(ids1, ids2);
}

template<typename TObject>
std::vector<int> getCommonIds(const MultipleFrameIndexedObjectsDatabase<TObject>* db, const std::vector<int>& indices) {
	assert (indices.size() > 1);
	std::vector<int> intersections = intersection(db->getAllObjectIdsInFrame(indices[0]), db->getAllObjectIdsInFrame(indices[1]));
	for (size_t i = 2; i < indices.size(); i++) {
		std::set<int> idset = db->getAllObjectIdsInFrame(indices[i]);
		std::vector<int> ids(idset.begin(), idset.end());
        std::vector<int> t = memdb_internal::intersection(intersections, ids);
		intersections = t;
	}
	return intersections;
}

template<typename TObject>
std::vector<std::pair<TObject, TObject> > getCommonIdObjects(const MultipleFrameIndexedObjectsDatabase<TObject>* db, int frameId1, int frameId2) {
	std::vector<int> commonIds = getCommonIds(db, frameId1, frameId2);
	std::vector<TObject> objs1 = db->getObjectsSubsetInFrame(frameId1, commonIds);
	std::vector<TObject> objs2 = db->getObjectsSubsetInFrame(frameId2, commonIds);
    std::vector<std::pair<TObject, TObject> > objPairs(commonIds.size());
	for (size_t i = 0; i < commonIds.size(); i++) {
		objPairs[i].first = objs1[i];
		objPairs[i].second = objs2[i];
	}
	return objPairs;
}

template<typename TObject>
void deleteObjectsWithSmallFrequence(MultipleFrameIndexedObjectsDatabase<TObject>* db, size_t minFrequence = 2) {
	std::set<int> objectIds = db->getAllObjectIds();
	std::set<int>::iterator iter = objectIds.begin();
	for ( ; iter != objectIds.end(); iter++) {
		if (db->getObjectFrequence(*iter) < minFrequence) {
			std::set<int> allFrameIds = db->getAllFrameIdsByObjectId(*iter);
			std::set<int>::iterator fiter = allFrameIds.begin();
			for (; fiter != allFrameIds.end(); fiter++) {
				db->deleteItem(*fiter, *iter);
			}
		}
	}
}

template<typename TObject>
std::set<int> getObjectIdsFromFrameSubsetWithMinimunFrequence(const MultipleFrameIndexedObjectsDatabase<TObject>* db, const std::set<int>& frameIds, int minFrequence = 2) {
	std::set<int> objectIds;
	std::set<int> allObjectIds = db->getAllObjectIds();
	std::set<int>::iterator oIter = allObjectIds.begin();
	for (; oIter != allObjectIds.end(); oIter++) {
		int objectId = *oIter;
		std::set<int>::const_iterator fIter = frameIds.begin();
		int freq = 0;
		for (; fIter != frameIds.end(); fIter++) {
			int frameId = *fIter;
			if (db->itemExists(frameId, objectId)) freq++;
		}
		if (freq >= minFrequence) {
			objectIds.insert(objectId);
		}
	}
	return objectIds;
}

template<typename TObject>
void deepClone(const IndexedObjectsDatabase<TObject>* src, IndexedObjectsDatabase<TObject>* dst) {
    typename IndexedObjectsDatabase<TObject>::ConstItemIterator* iter = src->getItemIterator();
	while (iter->hasNext()) {
        const typename IndexedObjectsDatabase<TObject>::Item* item = iter->next();
		dst->insertItem(item->id, item->object->clone());
	}
}

template<typename TObject>
void deepDelete(IndexedObjectsDatabase<TObject>* src) {
    typename IndexedObjectsDatabase<TObject>::ItemIterator* iter = src->getItemIterator();
	while (iter->hasNext()) {
        typename IndexedObjectsDatabase<TObject>::Item* item = iter->next();
		delete item->object;
	}
}

template<typename TObject>
TObject getMax(const MultipleFrameIndexedObjectsDatabase<TObject>* db) {
	if (db->size() == 0) { TObject o; return o; }
    typename MultipleFrameIndexedObjectsDatabase<TObject>::ConstItemIterator* iter = db->getItemIterator();
    const typename MultipleFrameIndexedObjectsDatabase<TObject>::Item* item = iter->next();
	TObject maxObject = item->object;
	while (iter->hasNext()) {
		item = iter->next();
		if (maxObject < item->object) {
			maxObject = item->object;
		}
	}
	return maxObject;
}

}
#endif // FVISION_MEMDB_UTILS_H_
