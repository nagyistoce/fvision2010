#ifndef FVISION_INDEX_ITERATOR_H_
#define FVISION_INDEX_ITERATOR_H_

namespace fvision {

class IndexIterator {
public:
	virtual ~IndexIterator(void) {}
	virtual bool hasNext() = 0;
	virtual int next() = 0;
	virtual IndexIterator* clone() const = 0;
};

class RangeIndexIterator : public IndexIterator {
public:
	//(1, 4, 1) means 1, 2, 3
	//[startIndex, endIndex)
	RangeIndexIterator(int startIndex, int endIndex, int interval = 1) {
		this->startIndex = startIndex;
		this->endIndex = endIndex;
		this->interval = interval;
		this->curIndex = startIndex;
	}

public:
	bool hasNext() {
		return curIndex < endIndex;		
	}

	int next() {
		int c = curIndex;
		curIndex += interval;
		return c;
	}

	IndexIterator* clone() const {
		return new RangeIndexIterator(startIndex, endIndex, interval);
	}
private:
	int startIndex;
	int endIndex;
	int interval;

	int curIndex;
};

//no end index
class StartIndexIterator : public IndexIterator {
public:
	StartIndexIterator(int startIndex = 0, int interval = 1) {
		this->startIndex = startIndex;
		this->interval = interval;
		this->curIndex = startIndex - interval;
	}

public:
	bool hasNext() {
		return true;
	}

	int next() {
		curIndex += interval;
		return curIndex;
	}

	IndexIterator* clone() const {
		return new StartIndexIterator(startIndex, interval);
	}
private:
	int startIndex;
	int interval;

	int curIndex;
};

}
#endif // FVISION_INDEX_ITERATOR_H_