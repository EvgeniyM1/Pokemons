#include "Counter.h"
#include "raylib.h"
#include <string>

Counter::Counter(int lim, int num) :
	count(num), limit(lim)
{}

bool Counter::isLimit()
{
	return count == limit;
}

int Counter::getNumber()
{
	return count;
}

void Counter::upCount()
{
	if(limit == 0 || count <= limit)
		++count;
	Invoke((float)count);
}

void Counter::downCount()
{
	if (count > 0)
		--count;
}

void Counter::setCount(int c)
{
	if(c >= 0)
		count = c;
}

void Counter::update(float b, float)
{
	if (int(b) == 1)
		upCount();
	else if ((int)b == 0)
		downCount();
	TraceLog(LOG_INFO, std::to_string(count).c_str()); 
}
