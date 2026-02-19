#pragma once
#include "Pokemon.h"
#include <vector>
#include<string>
#include "GameState.h"
#include "raylib.h"

class ActivePokemon
{
	Pokemon* pokemon;
	int* skillsRecharge;
	int countRecharges;
	int healthPointNow;
public:
	ActivePokemon();
	ActivePokemon(Pokemon* pok, int count_rechares = 2);

	void reduceRecharge();

	void setRecharge(int numberSkill, int recharge);

	bool isDied();

	Pokemon*& getPokemon();

	int getRecharge(int numberSkill);

	void setPokemon(Pokemon* lolololo);

	void RemovePokemon();

	void showInfo(int, int);

	void TakeDamage(int);
	int getHealthPointNow();

	void Draw(float x, float y, float size);
};
