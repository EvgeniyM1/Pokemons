#pragma once
#include "Pokemon.h"



bool Pokemon::isDied()
{

	if (healthPointNow <= 0)
	{
		return true;
	}
	return false;
}
Pokemon::Pokemon(std::string n, int hp, int def, int atk, Type tp, int kd, std::string str) :
	name(n), healthPoint(hp), defense(def), attack(atk), type(tp), rechargeSpecialSkill(kd)
{
	healthPointNow = healthPoint;
	texture = ResourceManager::GetTexture(str);
	id = index;
	++index;
};
Pokemon::Pokemon()
{
	id = -1;
	name = "";
	healthPoint = 0;
	healthPointNow = 0;
	defense = 0;
	attack = 0;
	type = Type::Null;
	rechargeSpecialSkill = 0;
	texture = Texture();
}

Pokemon::Pokemon(Pokemon* p)
{
	name = p->getName();
	healthPoint = p->getHealthPoint();
	healthPointNow = healthPoint;
	attack = p->getAttack();
	defense = p->getDefense();
	texture = p->getTexture();
	rechargeSpecialSkill = p->getRecharge();
	id = p->getId();
}

void Pokemon::TakeDamage(int damage)
{
	if (damage > defense)
		damage -= defense;
	else
		damage = 1;
	healthPointNow = healthPointNow - damage;
}

std::string Pokemon::getName()
{
	return name;
}

int Pokemon::getHealthPoint()
{
	return healthPoint;
}

int Pokemon::getHealthPointNow()
{
	return healthPointNow;
}

int Pokemon::getDefense()
{
	return defense;
}

int Pokemon::getAttack()
{
	return attack;
}

Type Pokemon::getType()
{
	return type;
}

int Pokemon::getRecharge()
{
	return rechargeSpecialSkill;
}

std::string Pokemon::getStrType()
{
	std::string str = "";
	switch (type)
	{
	case Type::Null:
		str = "Type not Found";
		break;
	case Type::Fire:
		str = "Fire";
		break;
	case Type::Water:
		str = "Water";
		break;
	case Type::Ground:
		str = "Ground";
		break;
	default:
		break;
	}
	return str;
}

Texture Pokemon::getTexture()
{
	return texture;
}

int Pokemon::getId()
{
	return id;
}

void Pokemon::setHealthPoint(int hp)
{
	if (hp <= 0) healthPointNow = 0;
	else if (hp >= healthPoint) healthPointNow = healthPoint;
	else
		healthPointNow = hp;
}

void Pokemon::upgradeAttack()
{
	attack += 2;
}

void Pokemon::upgradeDefense()
{
	defense += 2;
}

FirePokemon::FirePokemon(Pokemon* p) :
	Pokemon{ p }
{
	type = Type::Fire;
}

int FirePokemon::SpecialAttack(Pokemon*& enemy)
{
	int finalDamage = attack;
	if (enemy->getType() == Type::Ground)
		finalDamage *= 2;
	else if (enemy->getType() == Type::Water)
		finalDamage *= 0.7;
	else
		finalDamage *= 1.5;
	return finalDamage;
}

WaterPokemon::WaterPokemon(Pokemon* p) : 
	Pokemon{ p }
{
	type = Type::Water;
}

int WaterPokemon::SpecialAttack(Pokemon*& enemy)
{
	int finalDamage = attack;
	if (enemy->getType() == Type::Fire)
		finalDamage *= 1.5;
	else if (enemy->getType() == Type::Ground)
		finalDamage *= 0.9;
	else
		finalDamage *= 1.3;
	return finalDamage;
}

GroundPokemon::GroundPokemon(Pokemon* p) :
	Pokemon{ p }
{
	type = Type::Ground;
}

int GroundPokemon::SpecialAttack(Pokemon*& enemy)
{
	int finalDamage = attack;
	if (enemy->getType() == Type::Water)
		finalDamage *= 1.7;
	else if (enemy->getType() == Type::Fire)
		finalDamage *= 0.9;
	else
		finalDamage *= 1.3;
	return finalDamage;
}




