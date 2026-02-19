#include "BattleController.h"

int ChangePlayerPokemon()
{
	int choicePokemon = 0;
	if (IsKeyPressed(KEY_ONE))
		choicePokemon = 1;
	else if (IsKeyPressed(KEY_TWO))
		choicePokemon = 2;
	else if (IsKeyPressed(KEY_THREE))
		choicePokemon = 3;
	return choicePokemon - 1;
}

int changePlayerAction()
{
	int ch = 1;
	TraceLog(LOG_INFO, "Choose action 1 - attack, 2 - special attack");
	if (IsKeyPressed(KEY_ONE))
		ch = 1;
	else if (IsKeyPressed(KEY_THREE))
		ch = 2;
	return ch;
}

PlayerChoice playerAction(ActivePokemon& playerCurrentPokemon, ActivePokemon& enemyCurrentPokemon, std::string& mess)
{
	PlayerChoice playerChoose = PlayerChoice::NO_CHOOSED;
	int damage;
	if (IsKeyPressed(KEY_ONE))
	{
		TraceLog(LOG_INFO, "player\'s pokemon use attack");
		damage = playerCurrentPokemon.getPokemon()->getAttack() - playerCurrentPokemon.getPokemon()->getDefense();
		if (damage < 1) damage = 1;
		mess = "player\'s pokemon use attack and take " + std::to_string(damage) + " damage";

		//enemyCurrentPokemon.getPokemon()->TakeDamage(playerCurrentPokemon.getPokemon()->getAttack());
		enemyCurrentPokemon.TakeDamage(damage);
		PlaySound(ResourceManager::GetSound("attack-player"));
		playerChoose = PlayerChoice::CHOOSED;
	}
	else if (IsKeyPressed(KEY_TWO) && playerCurrentPokemon.getRecharge(2) <= 0)
	{
		TraceLog(LOG_INFO, "player\'s pokemon use special attack");
		//playerCurrentPokemon.getPokemon()->SpecialAttack(enemyCurrentPokemon.getPokemon());
		damage = playerCurrentPokemon.getPokemon()->SpecialAttack(enemyCurrentPokemon.getPokemon());
		damage -= damage % enemyCurrentPokemon.getPokemon()->getDefense();
		enemyCurrentPokemon.TakeDamage(damage);
		mess = "player\'s pokemon use special attackand take " + std::to_string(damage) + " damage";
		PlaySound(ResourceManager::GetSound("special-player"));

		playerCurrentPokemon.setRecharge(2, playerCurrentPokemon.getPokemon()->getRecharge());
		playerChoose = PlayerChoice::CHOOSED;
	}
	else if (IsKeyPressed(KEY_TWO) && playerCurrentPokemon.getRecharge(2) > 0)
	{
		TraceLog(LOG_INFO, "could down");
		mess = "skills have recharge " + std::to_string(playerCurrentPokemon.getRecharge(2)) + " raunds";
		//globalMessenger.WriteMessage()
		playerChoose = PlayerChoice::RECHARGE;
	}
	else if (IsKeyPressed(KEY_THREE))
	{
		playerChoose = PlayerChoice::CHANGE_POKEMON;
	}
	return playerChoose;
}

bool computerAction(ActivePokemon& playerCurrentPokemon, ActivePokemon& enemyCurrentPokemon, std::string& mess)
{
	bool choice = false;
	int damage;
	if (enemyCurrentPokemon.getRecharge(2) <= 0)
	{
		TraceLog(LOG_INFO, "II pokemon attacking with special attack");
		PlaySound(ResourceManager::GetSound("special-enemy"));
		//enemyCurrentPokemon.getPokemon()->SpecialAttack(playerCurrentPokemon.getPokemon());
		damage = enemyCurrentPokemon.getPokemon()->SpecialAttack(playerCurrentPokemon.getPokemon());
		damage -= damage % playerCurrentPokemon.getPokemon()->getDefense();
		mess = "II pokemon attacking with special attack and take " + std::to_string(damage) + " damage";
		playerCurrentPokemon.TakeDamage(damage);

		enemyCurrentPokemon.setRecharge(2, enemyCurrentPokemon.getPokemon()->getRecharge());
		choice = true;
	}
	else
	{
		damage = enemyCurrentPokemon.getPokemon()->getAttack();
		TraceLog(LOG_INFO, "II pokemon using attack");
		PlaySound(ResourceManager::GetSound("attack-enemy"));
		mess = "II pokemon attacking with attack and take " + std::to_string(damage) + " damage";
		playerCurrentPokemon.TakeDamage(damage);
		choice = true;
	}
	return choice;
}

void showInfo(Pokemon* playerPokemon, Pokemon* enemyPokemon)
{
	std::cout << '\n';
	std::cout << "ѕокемон игрока: HP = " << playerPokemon->getHealthPointNow() << '\n';
	std::cout << "ѕокемон »»: HP = " << enemyPokemon->getHealthPointNow() << '\n';
}
void showMessage(StateBattle& state, StateBattle& nowState, const StateBattle& nextState, std::string& mess, std::string text)
{
	state = StateBattle::MESSAGE;
	nowState = nextState;
	mess = text;
}


BattleController::BattleController() :
	playerPokemonPosition({ 100.F, 400.F }), enemyPokemonPosition({ 600.F, 400.F })
{
	choice = -1;
	activeChoice = -1;
	countLivedPlayerPokemon = 0;
	countLivedEnemyPokemon = 0;
	flagPlayer = false;
	flagEnemy = false;
	isSound = true;
	enemyAction = Action::NOTHING;
	playerState = Action::NOTHING;
	now = StateBattle::PAUSE;
	conclusion = EndBattle::CONTINUE;
	state = StateBattle::CHOOSE_POKEMON_PLAYER;
	countFrame = 0;
	countLetter = 0;
}

void BattleController::StartBattlePro()
{
	if (countLivedEnemyPokemon <= 0 && !(state == StateBattle::MESSAGE || state == StateBattle::EXIT))
	{
		if (isSound)
		{
			PlaySound(ResourceManager::GetSound("win-in-battle"));
			isSound = false;
		}
		playerPokemons[choice] = playerCurrentPokemon;
		//gameState = GameState::PLAYING;
		//conclusion = EndBattle::WIN;
		for (auto& el : playerPokemons)
		{
			std::string str;
			str = "el has " + std::to_string(el.getHealthPointNow()) + " hp";
			TraceLog(LOG_INFO, str.c_str());

			el.getPokemon()->setHealthPoint(el.getHealthPointNow());
			str = "pokemon has " + std::to_string(el.getPokemon()->getHealthPointNow()) + " hp";
			TraceLog(LOG_INFO, str.c_str());
		}
		showMessage(state, now, StateBattle::EXIT, message, "You Win!");
	}
	else if (countLivedPlayerPokemon <= 0)
	{
		TraceLog(LOG_INFO, "looose");
		gameState = GameState::LOSE;
		conclusion = EndBattle::LOSE;
	}

	ClearBackground(SKYBLUE);
	messanger.WriteMessage("Lived player's pokemons: " + std::to_string(countLivedPlayerPokemon), 10, 10);
	messanger.WriteMessage("Lived enemies's pokemons: " + std::to_string(countLivedEnemyPokemon), 500, 10);
	if (flagPlayer)
	{
		state = StateBattle::PAUSE;
		showMessage(state, now, StateBattle::CHOOSE_POKEMON_PLAYER, message, "Player\'s pokemon is died");
		flagPlayer = false;
	}
	if (flagEnemy)
	{
		state = StateBattle::PAUSE;
		showMessage(state, now, StateBattle::CHOOSE_POKEMON_ENEMY, message, "Enemy\'s pokemon is died");
		flagEnemy = false;
	}

	//отображение покемона игрока
	if (!playerCurrentPokemon.isDied())
	{
		playerCurrentPokemon.showInfo(10, 40);
		//DrawCircle(static_cast<int>(playerPokemonPosition.x),static_cast<int>(playerPokemonPosition.y), 100, BLUE);
		playerCurrentPokemon.Draw(playerPokemonPosition.x, playerPokemonPosition.y, 200);
	}
	//отображение покемона врага
	if (!enemyCurrentPokemon.isDied())
	{
		enemyCurrentPokemon.showInfo(550, 40);
		//DrawRectangle(static_cast<int>(enemyPokemonPosition.x), static_cast<int>(enemyPokemonPosition.y), 200, 200, RED);
		enemyCurrentPokemon.Draw(enemyPokemonPosition.x, enemyPokemonPosition.y, 200);
	}
	if (state == StateBattle::MESSAGE)
	{
		DrawRectangle(0, 650, 800, 150, BLACK);
		++countFrame;
		if (countFrame / messanger.getSpeed() == 1 && countLetter < (int)message.length())
		{
			countFrame = 0;
			++countLetter;
		}
		if (countLetter >= (int)message.length())
			messanger.WriteMessage("press R", 680, 780, WHITE);
			
		if (now == StateBattle::PAUSE)
			now = StateBattle::CHOOSE_POKEMON_PLAYER;
		if (!messanger.WriteMessagePro(message, state, now, countLetter))
		{
			countLetter = 0;
			countFrame = 0;
		}
	}
	else if (state == StateBattle::CHOOSE_POKEMON_PLAYER)
	{
		//выбор покемонов, если еще не вбыраны или покемон побежден
		if (playerCurrentPokemon.getHealthPointNow() <= 0)
		{
			message = "";
			for (int i{}; i < playerPokemons.size(); ++i)
				if (!playerPokemons[i].isDied())
					message += std::to_string(i + 1) + " - " + playerPokemons[i].getPokemon()->getName() + ", ";
			messanger.WriteMessage("Choose your pokemon: " + message);
			choice = ChangePlayerPokemon();
			if (choice != -1 && choice < 3 && playerPokemons[choice].getPokemon()->getHealthPointNow() > 0)
			{
				playerCurrentPokemon = playerPokemons[choice];
				playerState = Action::CHOSE_POKEMON;
			}
		}
		else
		{
			if (playerState == Action::CHOSE_POKEMON)
			{
				showMessage(state, now, StateBattle::CHOOSE_POKEMON_ENEMY, message, "Player choose " + playerCurrentPokemon.getPokemon()->getName());
			}
			else
			{
				state = StateBattle::CHOOSE_POKEMON_ENEMY;
			}
			flagEnemy = false;
			flagPlayer = false;
		}
	}
	else if (state == StateBattle::CHOOSE_POKEMON_ENEMY)
	{
		if (enemyCurrentPokemon.getHealthPointNow() <= 0)
		{
			enemyCurrentPokemon = enemyPokemons[enemyPokemons.size() - countLivedEnemyPokemon];
			std::string str = "Enemy choose " + enemyCurrentPokemon.getPokemon()->getName();
			playerState = Action::CHOOSE_ACTION;
			showMessage(state, now, StateBattle::PLAYER_CHOOSE_ACTION, message, str);
		}
		else
		{
			state = StateBattle::PLAYER_CHOOSE_ACTION;
			playerState = Action::CHOOSE_ACTION;
			TraceLog(LOG_INFO, "state = StateBattle::CHOOSE_ACTION");
		}
		playerState = Action::CHOOSE_ACTION;
	}

	//ход игрока
	else if (state == StateBattle::PLAYER_CHOOSE_ACTION)
	{
		if (playerState == Action::CHOOSE_ACTION)
		{
			PlayerChoice playerChoiceAction = playerAction(playerCurrentPokemon, enemyCurrentPokemon, message);
			messanger.WriteMessage("choose your action");
			if (playerChoiceAction == PlayerChoice::CHOOSED)
			{
				playerState = Action::NOTHING;
			}
			else if (playerChoiceAction == PlayerChoice::RECHARGE)
				showMessage(state, now, StateBattle::PLAYER_CHOOSE_ACTION, message, message);
			else if (playerChoiceAction == PlayerChoice::CHANGE_POKEMON)
			{
				state = StateBattle::CHANGE_POKEMON;
			}
		}
		else
		{
			showMessage(state, now, StateBattle::CHECK, message, message);
			playerState = Action::CHOOSE_ACTION;
		}
	}

	else if (state == StateBattle::CHANGE_POKEMON)
	{
		std::string str = "";
		for (int i{}; i < playerPokemons.size(); ++i)
			if(!playerPokemons[i].isDied())
				str += std::to_string(i + 1) + " - " + playerPokemons[i].getPokemon()->getName() + ", ";
		messanger.WriteMessage("Choose Pokemon: " + str);

		if (playerState == Action::CHOOSE_ACTION)
		{
			activeChoice = ChangePlayerPokemon();

			if (activeChoice != -1 && activeChoice < 3 && playerPokemons[activeChoice].getPokemon()->getHealthPointNow() > 0)
			{
				playerState = Action::CHOSE_POKEMON;
			}
		}
		else
		{
			if (activeChoice == choice)
			{
				showMessage(state, now, StateBattle::PLAYER_CHOOSE_ACTION, message, "You choose active pokemon!");
			}
			else
			{
				playerPokemons[choice] = playerCurrentPokemon;
				choice = activeChoice;
				playerCurrentPokemon = playerPokemons[activeChoice];
				showMessage(state, now, StateBattle::CHECK, message, "Player choose " + playerCurrentPokemon.getPokemon()->getName());
			}
			playerState = Action::CHOOSE_ACTION;
		}
	}

	else if (state == StateBattle::CHECK)
	{
		TraceLog(LOG_INFO, "CHEEECK");
		//messanger.WriteMessage("Checking");
		//проверка на то, что покемоны живы
		if (playerCurrentPokemon.isDied() || enemyCurrentPokemon.isDied())
		{
			if (playerCurrentPokemon.isDied())
			{
				--countLivedPlayerPokemon;
				playerCurrentPokemon.getPokemon()->setHealthPoint(0);
				playerPokemons[choice] = playerCurrentPokemon;
				flagPlayer = true;

			}
			else
			{
				--countLivedEnemyPokemon;
				flagEnemy = true;
			}
			return;
		}
		state = StateBattle::CHOOSE_COMPUTER;
		enemyAction = Action::CHOOSE_ACTION;
		TraceLog(LOG_INFO, "StateBattle::CHOOSE_COMPUTER");
	}

	//ход компьютера
	else if (state == StateBattle::CHOOSE_COMPUTER)
	{
		if (enemyAction == Action::CHOOSE_ACTION)
		{
			if (computerAction(playerCurrentPokemon, enemyCurrentPokemon, message))
			{
				enemyAction = Action::NOTHING;
			}
		}
		else
		{
			showMessage(state, now, StateBattle::END, message, message);
			enemyAction = Action::CHOOSE_ACTION;
		}
	}
	else if (state == StateBattle::END)
	{
		TraceLog(LOG_INFO, "END");
		//проверка на то, что покемоны живы
		if (!flagPlayer && playerCurrentPokemon.isDied())
		{
			--countLivedPlayerPokemon;
			flagPlayer = true;
			playerCurrentPokemon.getPokemon()->setHealthPoint(0);
			playerPokemons[choice] = playerCurrentPokemon;
		}
		if (!flagEnemy && enemyCurrentPokemon.isDied())
		{
			--countLivedEnemyPokemon;
			flagEnemy = true;
		}

		//снижение перезар€дки навыков
		for (int i{}; i < playerPokemons.size(); ++i)
			playerPokemons[i].reduceRecharge();
		for (int i{}; i < enemyPokemons.size(); ++i)
			enemyPokemons[i].reduceRecharge();

		state = StateBattle::PLAYER_CHOOSE_ACTION;
	}
	else if (state == StateBattle::EXIT)
	{
		gameState = GameState::PLAYING;
		conclusion = EndBattle::WIN;
	}
}

void BattleController::SetPokemons(std::vector<Pokemon*>& pl, std::vector<Pokemon*>& en)
{
	int countDied = 0;
	playerPokemons.clear();
	enemyPokemons.clear();
	for (Pokemon*& el : pl)
	{

		if (el->isDied())
			countDied++;
		playerPokemons.push_back(el);
		//std::string str = "el has " + std::to_string(el->getHealthPointNow()) + " hp";
		//TraceLog(LOG_INFO, str.c_str());
	}
	for (auto& el : en)
		if (!el->isDied())
			enemyPokemons.push_back(el);

	countLivedPlayerPokemon = playerPokemons.size() - countDied;
	countLivedEnemyPokemon = enemyPokemons.size();

	conclusion = EndBattle::CONTINUE;
	state = StateBattle::CHOOSE_POKEMON_PLAYER;
	flagPlayer = false;
	flagEnemy = false;
	isSound = true;
	choice = -1;
	playerCurrentPokemon.RemovePokemon();
	enemyCurrentPokemon.RemovePokemon();
	countFrame = 0;
	countLetter = 0;
}

EndBattle BattleController::GetConclusion()
{
	return conclusion;
}

