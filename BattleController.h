#pragma once
#include "ActivePokemon.h"
#include "Message.h"

enum class PlayerChoice { CHOOSED, NO_CHOOSED, RECHARGE, CHANGE_POKEMON };
enum class Action { CHOSE_POKEMON, CHOOSE_ACTION, NOTHING };
enum class EndBattle { WIN, LOSE, CONTINUE };

class BattleController
{
	int choice;
	int activeChoice;
	int countLivedPlayerPokemon;
	int countLivedEnemyPokemon;
	int countFrame;
	int countLetter;

	bool flagPlayer;
	bool flagEnemy;
	bool isSound;

	std::string message;
	Message messanger{2};

	ActivePokemon playerCurrentPokemon, enemyCurrentPokemon;

	Action playerState;
	Action enemyAction;
	StateBattle state;
	StateBattle now;
	EndBattle conclusion;

	const Vector2 playerPokemonPosition;
	const Vector2 enemyPokemonPosition;

	std::vector<ActivePokemon> playerPokemons;
	std::vector<ActivePokemon> enemyPokemons;
public:
	BattleController();
	void StartBattlePro();

	void SetPokemons(std::vector<Pokemon*>&, std::vector <Pokemon*>&);

	EndBattle GetConclusion();
};

int ChangePlayerPokemon();
int changePlayerAction();
PlayerChoice playerAction(ActivePokemon&, ActivePokemon&, std::string&);
bool computerAction(ActivePokemon&, ActivePokemon&, std::string&);
void showInfo(Pokemon*, Pokemon*);
void showMessage(StateBattle&, StateBattle&, const StateBattle&, std::string&, std::string);