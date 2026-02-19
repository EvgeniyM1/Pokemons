#pragma once

#include "ActivePokemon.h"

enum class StateBattle { CHOOSE_POKEMON_PLAYER, CHOOSE_POKEMON_ENEMY, PLAYER_CHOOSE_ACTION, CHECK, CHOOSE_COMPUTER, END, PAUSE, MESSAGE, CHANGE_POKEMON, EXIT };

struct Message
{
	int key;
	Vector2 postiton{ 10.F, 750.F };
	int speed;

public:
	Message(int sp, int st = KEY_R);
	bool WriteMessage(std::string, StateBattle&, const StateBattle&);
	void WriteMessage(std::string message);
	void WriteMessage(std::string message, int positionX, int positionY, Color color = BLACK);

	bool WriteMessagePro(std::string, StateBattle&, const StateBattle&, int&);

	int getSpeed();
};