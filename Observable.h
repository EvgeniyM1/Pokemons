#pragma once
#include <vector>

struct Observer
{
	virtual void update(float = 0, float = 0) = 0;
};

struct Subject
{
	std::vector<Observer*> observers;

	void addObserver(Observer*);
	void deleteObserver(int);

	void Invoke(float = 0, float = 0);
};
