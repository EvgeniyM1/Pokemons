#pragma once
#include "Board.h"
#include "Drawer.h"
#include "Animation.h"
#include "Pokemon.h"
#include "Observable.h"

const int CELLSIZE = SCREEN_WIDTH / SIZE;

class Player : public Drawable, public Subject
{
	Vector2 position;
	Vector2 newPosition;
	float moveSpeed = 1.F;
	Texture texture;
	std::unordered_map<std::string, Texture> textures;
	std::vector<Pokemon*> pokemons;

	std::string lastKey;
	int lastKeyInt;
	Animation animator;

	std::vector<Texture> topDirection;
	std::vector<Texture> rightDirection;
	std::vector<Texture> leftDirection;
	std::vector<Texture> bottomDirection;

	Vector2 SetNewPosition(Board&);

public:
	Player(std::string str);

	Vector2 getPosition();
	std::vector<Pokemon*>& getPokemons();

	void Move(Board&);
	
	void Draw() override;


	void setStartPosition();
	void setStartPosition(float, float);
	void addPokemon(Pokemon* p);
	void clearPokemons();
};