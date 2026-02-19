#pragma once

class Drawable
{
public:
	virtual void Draw() = 0;
};

class Restartable
{
	virtual void Restart() = 0;
};