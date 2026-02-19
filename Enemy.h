#pragma once
#include "raylib.h"
#include "Drawer.h"
#include "Resources.h"
#include "Pokemon.h"

class Enemy : public Drawable
{
	Vector2 position;
	Texture texture;
	std::vector<Pokemon*> pokemons;

public:
	Enemy(int x, int y, std::string str);
	~Enemy()
	{
		//deletePokemons();
	};
	Enemy();

	void addPokemon(Pokemon*&);

	void deletePokemons();
	
	void Draw() override;

	Vector2 getPosition();
	std::vector<Pokemon*> getPokemons();
};