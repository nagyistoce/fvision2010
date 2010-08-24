#pragma once

#include "IndexedObjectsDatabase.h"
#include "MultipleFrameIndexedObjectsDatabase.h"

namespace fvision {

template<typename TObject>
bool isDatabaseEqual(const IndexedObjectsDatabase<TObject>* lhs, const IndexedObjectsDatabase<TObject>* rhs) {
	if (lhs->size() != rhs->size()) return false;
    typename IndexedObjectsDatabase<TObject>::ConstItemIterator* iter = lhs->getItemIterator();
	while(iter->hasNext()) {
        const typename IndexedObjectsDatabase<TObject>::Item* item = iter->next();
		if (!(item->object == rhs->getObjectById(item->id))) return false;
	}
	return true;
}

template<typename TObject>
bool isDatabaseEqual(
	const MultipleFrameIndexedObjectsDatabase<TObject>* lhs, 
	const MultipleFrameIndexedObjectsDatabase<TObject>* rhs) 
{
	if (lhs->size() != rhs->size()) return false;
    typename MultipleFrameIndexedObjectsDatabase<TObject>::ConstItemIterator* iter = lhs->getItemIterator();
	while(iter->hasNext()) {
        const typename MultipleFrameIndexedObjectsDatabase<TObject>::Item* item = iter->next();
		if (!(item->object == rhs->getObjectByFrameIdAndObjectId(item->frameId, item->objectId))) return false;
	}
	return true;
}

}
