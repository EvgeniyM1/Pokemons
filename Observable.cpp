#include "Observable.h"

void Subject::addObserver(Observer* observer)
{
	observers.push_back(observer);
}

void Subject::deleteObserver(int index)
{
	if (index < observers.size())
		observers.erase(observers.begin() + index);
}

void Subject::Invoke(float x, float y)
{
	for (auto& ob : observers)
	{
		ob->update(x, y);
	}
}