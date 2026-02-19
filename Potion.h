#pragma once
#include "raylib.h"
#include "Drawer.h"
//#include "Player.h"

class Potion : public Drawable
{
protected:
	Vector2 position;
	Texture texture;

public:
	Potion(Vector2 pos, Texture tex);

	void Draw()override;
	virtual float Applay() = 0;
};

class HealthPotion : public Potion
{
	int healAmout;
public:
	HealthPotion(Vector2 pos, Texture tex, int health);
	float Applay()override;
	Vector2 getPosition();
};