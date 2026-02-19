#include "ActivePokemon.h"

ActivePokemon::ActivePokemon()
{
	pokemon = nullptr;
	countRecharges = 2;
	skillsRecharge = nullptr;
	healthPointNow = 0;
}
ActivePokemon::ActivePokemon(Pokemon* pok, int count_rechares)
{
	pokemon = pok;
	countRecharges = count_rechares;
	skillsRecharge = new int[countRecharges]();
	healthPointNow = pokemon->getHealthPointNow();
}

void ActivePokemon::reduceRecharge()
{
	if (pokemon == nullptr || isDied())
		return;
	for (int i{}; i < countRecharges; ++i)
		skillsRecharge[i] = skillsRecharge[i] - 1;
}

void ActivePokemon::setRecharge(int numberSkill, int recharge)
{
	skillsRecharge[numberSkill - 1] = recharge;
}

bool ActivePokemon::isDied()
{
	if (pokemon != nullptr)
		return healthPointNow <= 0;
	else
		return true;
}

Pokemon*& ActivePokemon::getPokemon()
{
	return pokemon;
}

int ActivePokemon::getRecharge(int numberSkill)
{
	return skillsRecharge[numberSkill - 1];
}

void ActivePokemon::setPokemon(Pokemon* lolololo)
{
	pokemon = lolololo;
	if (pokemon != nullptr)
	{
		countRecharges = 2;
		skillsRecharge = new int[countRecharges]();
	}
}

void ActivePokemon::RemovePokemon()
{
	healthPointNow = 0;
}

void ActivePokemon::showInfo(int x, int y)
{
	std::string str;
	str = "Pokemon: " + pokemon->getName() + ", " + pokemon->getStrType();
	DrawText(str.c_str(), x, y, 20, BLACK);

	str = "HP: " + std::to_string(healthPointNow) + " / " + std::to_string(pokemon->getHealthPoint());
	DrawText(str.c_str(), x, y + 30, 20, BLACK);
}

void ActivePokemon::TakeDamage(int damage)
{
	int defense = pokemon->getDefense();
	if (damage > defense)
		damage -= defense;
	else
		damage = 1;
	healthPointNow = healthPointNow - damage;
	if (healthPointNow < 0) healthPointNow = 0;
}

int ActivePokemon::getHealthPointNow()
{
	return healthPointNow;
}

void ActivePokemon::Draw(float x, float y, float size)
{
	Texture texture = pokemon->getTexture();
	DrawTexturePro(
		texture,
		{ 0, 0, (float)texture.width, (float)texture.height },
		{
			x /** size*/,
			y /** size*/,
			(float)size,
			(float)size
		},
		{ 0,0 }, 0.0f, WHITE);
}
//int main()
//{
//	setlocale(0, "");
//	std::vector<Pokemon*> player;
//	std::vector<Pokemon*> enemy;
//	Pokemon* p1 = new FirePokemon("pl_1", 100, 5, 45, 4), * p2 = new WaterPokemon("pl_2", 120, 4, 7, 4), * p3 = new GroundPokemon("pl_3", 110, 2, 15, 5);
//	Pokemon* p4 = new FirePokemon("en_1", 100, 3, 50, 4), * p5 = new WaterPokemon("en_2", 120, 4, 7, 4), * p6 = new GroundPokemon("en_3", 110, 3, 15, 5);
//	player.push_back(p1);
//	player.push_back(p2);
//	player.push_back(p3);
//	enemy.push_back(p4);
//	enemy.push_back(p5);
//	enemy.push_back(p6);
//	//if(IsKeyPressed(KEY_RIGHT)){}
//
//	// Инициализация окна
//	const int screenWight = 800;
//	const int screenHight = 800;
//
//	//SetTargetFPS(60);
//
//	//InitWindow(screenWight, screenHight, "Window");
//
//	StartBattlePro(player, enemy);
//
//	player.clear();
//	enemy.clear();
//	delete p1, p2, p3, p4, p5, p6;
//}

