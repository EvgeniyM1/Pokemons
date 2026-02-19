#include "Enemy.h"

Enemy::Enemy(int x, int y, std::string str) :
	position{ static_cast<float>(x), static_cast<float>(y) }, texture(ResourceManager::GetTexture(str))
{
}

Enemy::Enemy() :
	position{ 0.F, 0.F }, texture(ResourceManager::GetTexture("enemy"))
{
}

void Enemy::addPokemon(Pokemon*& pokemon)
{
	Pokemon* p;
	switch (pokemon->getType())
	{
	case Type::Fire:
		p = new FirePokemon(pokemon);
		break;
	case Type::Water:
		p = new WaterPokemon(pokemon);
		break;
	case Type::Ground:
		p = new GroundPokemon(pokemon);
		break;
	default:
		p = new FirePokemon(pokemon);
		break;
	}
	pokemons.push_back(p);
}

void Enemy::deletePokemons()
{
	for (auto& el : pokemons)
	{
		delete el;
		el = nullptr;
	}
	pokemons.clear();
}

void Enemy::Draw()
{
	DrawTexturePro(
		texture,
		{ 0, 0, (float)texture.width, (float)texture.height },
		{
			position.x * 100,
			position.y * 100,
			(float)100,
			(float)100
		},
		{ 0,0 }, 0.0f, WHITE);
}

Vector2 Enemy::getPosition()
{
	return position;
}

std::vector<Pokemon*> Enemy::getPokemons()
{
	return pokemons;
}
