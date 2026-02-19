#pragma once
#include "Observable.h"

class Counter : public Observer, public Subject
{
	int count;
	int limit;
public:
	Counter(int = 0, int = 0);

	bool isLimit();

	int getNumber();
	
	void upCount();
	void downCount();

	void setCount(int);

	void update(float, float) override;
};