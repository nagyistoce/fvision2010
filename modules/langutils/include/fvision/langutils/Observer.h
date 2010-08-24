#ifndef FVISION_OBSERVER_H_
#define FVISION_OBSERVER_H_

#include <vector>

namespace fvision { 

class Observer;

class Observable {
public:
	virtual ~Observable();

public:
	void addObserver(Observer* observer);
	void notify();

private:
	std::vector<Observer*> observers;
};

class Observer {
public:
	virtual ~Observer() {}

public:
	virtual void update(Observable* observable) = 0;
};
}
#endif // FVISION_OBSERVER_H_