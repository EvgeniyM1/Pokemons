#pragma once
#include<iostream>
#include <raylib.h>
#include <vector>
#include <string>
#include "Resources.h"

enum class Type { Null, Fire, Water, Ground };

class Pokemon
{
protected:
	int id;
	std::string name;
	int healthPoint;
	int healthPointNow;
	int defense;
	int attack;
	int rechargeSpecialSkill;
	Type type;
	Texture texture;

	static int index;

public:
	bool isDied();

	Pokemon(std::string n, int hp, int def, int atk, Type tp, int kd, std::string );
	Pokemon();
	Pokemon(Pokemon*);
	virtual ~Pokemon() {};

	void TakeDamage(int damage);

	virtual int SpecialAttack(Pokemon*& enemy) = 0;

	std::string getName();
	int getHealthPoint();
	int getHealthPointNow();
	int getDefense();
	int getAttack();
	Type getType();
	int getRecharge();
	std::string getStrType();
	Texture getTexture();
	int getId();

	void setHealthPoint(int);
	void upgradeAttack();
	void upgradeDefense();
};

inline int Pokemon::index = 0;

class FirePokemon : public Pokemon
{
public:
	FirePokemon(std::string n, int hp, int def, int atk, int kd, std::string str) :
		Pokemon(n, hp, def, atk, Type::Fire, kd, str)
	{}
	FirePokemon(Pokemon*);
	//FirePokemon() :
	//	Pokemon("", 0, 0, 0, Type::Fire, 0)
	//{}

	int SpecialAttack(Pokemon*& enemy) override;
};

class  WaterPokemon : public Pokemon
{
public:
	WaterPokemon(std::string n, int hp, int def, int atk, int kd, std::string str) :
		Pokemon(n, hp, def, atk, Type::Water, kd, str)
	{}
	WaterPokemon(Pokemon*);

	int SpecialAttack(Pokemon*& enemy) override;
};

class GroundPokemon : public Pokemon
{
public:
	GroundPokemon(std::string n, int hp, int def, int atk, int kd, std::string str) :
		Pokemon(n, hp, def, atk, Type::Ground, kd, str)
	{}
	GroundPokemon(Pokemon*);

	int SpecialAttack(Pokemon*& enemy) override;
};
