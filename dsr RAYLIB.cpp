#include "raylib.h"
#include <string>
#include <locale>
#include <cmath>
#include <vector>
#include "ActivePokemon.h"
#include "Player.h"
#include "GameState.h"
#include "Drawer.h"
#include "Enemy.h"
#include "BattleController.h"
#include "Cells.h"
#include "Potion.h"
#include "Counter.h"
#pragma once

static void createEnemies(std::vector<Enemy>& v, const Board& board, std::vector<Pokemon*>& p)
{
	for (int row{}; row < SIZE; ++row)
		for (int col{}; col < SIZE; ++col)
			if (board.isObject(col, row) == Cells::ENEMY)
			{
				Enemy en{ col, row, "enemy" };
				for (int i{}; i < 3; ++i)
					en.addPokemon(p[GetRandomValue(0, p.size() - 1)]);
				v.push_back(en);
			}
}

static void playMusic(Music& music)
{
	if (!IsMusicStreamPlaying(music))
	{
		PlayMusicStream(music);
	}
	else
	{
		UpdateMusicStream(music);
	}
}

static void createDrawable(std::vector<Drawable*>& drawable, std::vector<Enemy>& enemies)
{
	
	for (Enemy& el : enemies)
	{
		Enemy* en = new Enemy(el);
		drawable.push_back(en);
	}
}

static void clickOnCell(int& pastIndex, std::vector<PokemonsCells>& c, Counter& counter)
{
	Vector2 mouse = GetMousePosition();
	for (int i{}; i < c.size(); ++i)
	{
		if (CheckCollisionPointRec(mouse, c[i].getRectangle()))
		{
			if (pastIndex != -1)
			{
				c[pastIndex].setDefaultColor();
				counter.deleteObserver(0);
			}
			c[i].setColor(YELLOW);
			counter.addObserver(&c[i]);
			pastIndex = i;
		}
	}
}

static void showInfo(int x, int y, Pokemon* pokemon)
{
	int widthBar = 150;
	int widthCard = 200, heightCard = 250;
	int widthTexture = 150, heightTexture = 150;
	Texture texture = pokemon->getTexture();
	float hp = static_cast<float>(pokemon->getHealthPointNow()), maxHp = static_cast<float>(pokemon->getHealthPoint());

	DrawRectangle(x, y, widthCard, heightCard, GRAY);
	DrawTexturePro(texture, { 0.F, 0.F, (float)texture.width, (float)texture.height }, { (float)(x + (widthCard - widthTexture)), (float)(y), (float)widthTexture, (float)heightTexture}, {0.F, 0.F}, 0.F, WHITE);

	DrawRectangle(x + (widthCard - widthBar) / 2, y + heightCard * 7 / 10, widthBar, 20, DARKGRAY);
	DrawRectangle(x + (widthCard - widthBar) / 2, y + heightCard * 7 / 10, ((float)hp / maxHp) * widthBar, 20, GREEN);
	DrawText(TextFormat("%d / %d", (int)hp, (int)maxHp), x + 50, y + heightCard * 7 / 10, 20, BLACK);
}

static void saveInfo(std::vector<Enemy> enemies, const std::vector<Pokemon*>& playerPokemons)
{
	std::ofstream saveState{ "saves/info.txt" };
	if (!saveState.is_open()) return;
	saveState << (int)enemies.size() << '\n';
	for (auto& pokemon : playerPokemons)
	{
		saveState << pokemon->getId() << " ";
	}
	saveState << '\n';
	for (auto& pokemon : playerPokemons)
	{
		saveState << pokemon->getHealthPointNow() << " ";
	}
	saveState << '\n';
}

int main()
{

	// Инициализация окна
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Window");
	InitAudioDevice();
	ResourceManager::Load();

	bool isSave = false;
	const std::string startFile = "saves/start.txt";
	const std::string infoFile = "saves/info.txt";
	std::ifstream savingInfo{ infoFile };
	if (savingInfo.is_open())
	{
		int enemiesCount;
		savingInfo >> enemiesCount;
		if (enemiesCount > 0)
		{
			isSave = true;
		}
		else
		{
			savingInfo.close();
		}

	}

	Font font = LoadFontEx("resources/arial.ttf", 32, 0, 0);
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

	Board board{"saves/map.txt"};
	if (isSave)
	{
		board.Restart(board.getFileForSave());
	}
	else
	{
		board.Restart("saves/start.txt");
	}
	std::vector<Enemy> enemies;


	std::locale::global(std::locale(""));
	std::vector<Pokemon*> pokemons{ new FirePokemon("Flareon", 120, 5, 40, 4, "flareon"),
		new WaterPokemon("Squirtle", 120, 6, 40, 3, "squirtle"),
		new GroundPokemon("Bulbasaur", 140, 13, 40, 4, "bulbasaur"),
		new FirePokemon("Charmander", 100, 3, 50, 4, "charmander"),
		new WaterPokemon("Litten", 130, 5, 30, 3, "litten"),
		new GroundPokemon("Cubone", 150, 12, 25, 4, "cubone")
	};

	std::vector<PokemonsCells> cells;
	std::vector<Pokemon*> ChangingPokemons;
	std::vector<PokemonsCells> chooseCells;

	BattleController controller{};

	Player player("player");

	player.setStartPosition(board.getStartPosition().x, board.getStartPosition().y);
	std::vector<Drawable*> drawable{};
	player.addObserver(&board);

	//загрузка музыки
	Music playingMusic = ResourceManager::GetMusic("playing");
	Music battleMusic = ResourceManager::GetMusic("battle");
	Music loseMusic = ResourceManager::GetMusic("lose");
	Music startMusic = ResourceManager::GetMusic("start");

	// Задача текстур
	Texture2D restBut = ResourceManager::GetTexture("restart");
	Texture2D extBut = ResourceManager::GetTexture("exit");

	// Состояние игры
	gameState = GameState::START;
	bool isPlaying = false, isBattleStarted = true, foundEnemy = false, isSoundWin = false;

	const float firstPositionX = 100;
	const float firstPositionY = 300;
	const float width = 100;
	const float height = 100;
	const int maxCountPokemons = 3;

	Button Start("START", 200, 50, { 250, 400 }, 30, GREEN),
		Continue{ "NEXT", 200, 50, {600.F, 750.F}, 20, GREEN },
		BackToChangingPokemons("BACK", 200, 50, { 0.F, 750.F }, 20, GREEN),
		BackToStart("BACK", 200, 50, { 0.F, 750.F }, 20, GREEN),
		StartPlaying("PLAY", 200, 50, { 600.F, 750.F }, 20, GREEN),
		ContinueGame("CONTINUE", 200, 50, {250, 500}, 30, YELLOW),
		exitButton("EXIT", 200, 50, { 250, 600 }, 30, RED),
		returnMenu("MENU", 200, 50, { 250, 400 }, 30, YELLOW);

	int numEnemies = 0, col = 0, row = 0, index = 0, pastIndex = -1, countPokemons = 0;
	Counter counter{3};
		

	for (int i{}; i < pokemons.size(); ++i)
	{
		float posY = firstPositionY + row * (height + 10);;
		float posX = firstPositionX + (col * width * 1.5F);
		++col;
		if (col >= SCREEN_WIDTH / (width * 2))
		{
			col = 0;
			++row;
		}
		cells.push_back({ posX, posY, width, height, pokemons[i], RED });
		cells[i].addObserver(&counter);
	}
	bool flag = true;

	//Установка FPS
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		if (gameState == GameState::START || gameState == GameState::CHANGING_POKEMONS || gameState == GameState::PREVIEW)
		{
			playMusic(startMusic);
		}
		if(gameState == GameState::START)
		{
			if (isPlaying)
				isPlaying = false;
		}
		if (gameState == GameState::PLAYING || gameState == GameState::PAUSE)
		{
			if (!isPlaying)
				isPlaying = true;
			playMusic(playingMusic);
			if (IsKeyPressed(KEY_P) && gameState == GameState::PAUSE)
				gameState = GameState::PLAYING;
			else if (IsKeyPressed(KEY_P) && gameState == GameState::PLAYING)
				gameState = GameState::PAUSE;
		}
		if (gameState == GameState::PLAYING)
		{
			player.Move(board);
			playMusic(playingMusic);
		}

		BeginDrawing();

		if (gameState == GameState::START)
		{
			ClearBackground(RAYWHITE);
			DrawText("Pockemons Game", 250, 250, 30, BLACK);
			Start.Draw(GameState::CHANGING_POKEMONS);
			if(isSave)
				ContinueGame.Draw(GameState::CONTINUE);
			else
			{
				DrawText("Not Save", 300, 500, 20, LIGHTGRAY);
			}

			exitButton.Draw();
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Vector2 mousePos = GetMousePosition();
				if (CheckCollisionPointRec(mousePos, exitButton.getRectangle()))
				{
					break;
				}
			}
		}
		else if (gameState == GameState::CONTINUE)
		{
			int indexPokemons, hp;
			if(player.getPokemons().empty())
			{
				for (int i{}; i < 3; ++i)
				{
					savingInfo >> indexPokemons;
					player.addPokemon(pokemons[indexPokemons]);
				}
				for (int i{}; i < 3; ++i)
				{
					savingInfo >> hp;
					player.getPokemons()[i]->setHealthPoint(hp);
				}
			}
			if(enemies.empty())
				createEnemies(enemies, board, pokemons);
			if (drawable.empty())
			{
				drawable.push_back(&board);
				drawable.push_back(&player);
				createDrawable(drawable, enemies);
			}
			else if((int)drawable.size() == 2)
				createDrawable(drawable, enemies);
			gameState = GameState::PLAYING;
			savingInfo.close();
		}
		else if (gameState == GameState::CHANGING_POKEMONS)
		{
			ClearBackground(LIGHTGRAY);
			DrawText("Change your pokemons", 250, 200, 30, BLACK);
			for (int i{}; i < cells.size(); ++i)
				cells[i].Draw();
			DrawText(("pokemons selected: " + std::to_string(counter.getNumber())).c_str(), 10, 700, 20, BLACK);
			if (counter.getNumber() == maxCountPokemons)
				Continue.Draw(GameState::PREVIEW);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				clickOnCell(pastIndex, cells, counter);
			}
			if (pastIndex != -1)
				cells[pastIndex].showInfo();
			BackToStart.Draw(GameState::START);
		}
		else if (gameState == GameState::PREVIEW)
		{
			if (flag)
			{
				float posX = firstPositionX;
				int k = 0;
				for (int i{}; i < cells.size(); ++i)
				{
					if (cells[i].getSelected())
					{
						ChangingPokemons.push_back(cells[i].getPokemon());
						chooseCells.push_back(cells[i]);
						chooseCells[k++].setPosition(posX, firstPositionY);
						posX += width * 1.5F;
					}
				}
				flag = false;
			}
			ClearBackground(LIME);
			BackToChangingPokemons.Draw();
			StartPlaying.Draw();
			for (int i{}; i < chooseCells.size(); ++i)
			{
				chooseCells[i].Draw();
			}

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Vector2 mouse = GetMousePosition();
				if (CheckCollisionPointRec(mouse, BackToChangingPokemons.getRectangle()))
				{
					BackToChangingPokemons.OnClick(GameState::CHANGING_POKEMONS);
					ChangingPokemons.clear();
					chooseCells.clear();
					flag = true;
				}
				if (CheckCollisionPointRec(mouse, StartPlaying.getRectangle()))
				{
					board.Restart(startFile);
					if (!player.getPokemons().empty())
						player.clearPokemons();
					enemies.clear();
					drawable.clear();
					StartPlaying.OnClick(GameState::PLAYING);
					player.setStartPosition();

					for (int i{}; i < ChangingPokemons.size(); ++i)
						player.addPokemon(ChangingPokemons[i]);

					ChangingPokemons.clear();
					chooseCells.clear();
					flag = true;
					for (auto& el : player.getPokemons())
						el->setHealthPoint(el->getHealthPoint());
					createEnemies(enemies, board, pokemons);
					drawable.push_back(&board);
					drawable.push_back(&player);
					createDrawable(drawable, enemies);

				}
			}
		}
		else if (gameState == GameState::PLAYING)
		{
			// Отрисовка шахматки
			for (int i{}; i < drawable.size(); ++i)
				drawable[i]->Draw();

			ClearBackground(RAYWHITE);
			DrawText("PAUSE[P]", 10, 10, 20, YELLOW);
		}
		else if (gameState == GameState::PAUSE)
		{
			ClearBackground(LIGHTGRAY);

			for (int i{}; i < 3; ++i)
				showInfo(205 * i + 95, 10, player.getPokemons()[i]);
			returnMenu.Draw(GameState::START);
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Vector2 mousePos = GetMousePosition();
				if (CheckCollisionPointRec(mousePos, returnMenu.getRectangle()))
				{
					board.writeSave(player.getPosition().x, player.getPosition().y);
					saveInfo(enemies, player.getPokemons());
					isSave = true;
					StopMusicStream(startMusic);
					StopMusicStream(playingMusic);
				}

			}
			DrawText("RESUME[P]", 600, 750, 20, BLACK);
		}
		else if (gameState == GameState::VICTORY)
		{
			ClearBackground(GREEN);
			if (!isSoundWin)
			{
				PlaySound(ResourceManager::GetSound("victory"));
				isSoundWin = true;
			}
			DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.5f));
			DrawTextEx(font, "Win!!!", { SCREEN_WIDTH / 2 - MeasureTextEx(font, "WIN!!!", 50, 2).x / 2, 200 }, 50, 2, GREEN);

			Rectangle rectB = { SCREEN_WIDTH / 2 - 150, 300, 300, 80 };

			DrawTexturePro(
				restBut,
				{ 0, 0, (float)restBut.width, (float)restBut.height },
				rectB,
				{ 0,0 },
				0.0f,
				WHITE
			);
			Rectangle extB = { SCREEN_WIDTH / 2 - 150, 400, 300, 80 };
			DrawTexturePro(
				extBut,
				{ 0, 0, (float)extBut.width, (float)extBut.height },
				extB,
				{ 0,0 },
				0.0f,
				WHITE
			);

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Vector2 mousePos = GetMousePosition();
				if (CheckCollisionPointRec(mousePos, rectB))
				{
					gameState = GameState::RESTART;
				}
				else if (CheckCollisionPointRec(mousePos, extB))
				{
					gameState = GameState::START;
					isSave = false;
					StopMusicStream(startMusic);
					drawable.clear();
					enemies.clear();
				}
				isSoundWin = false;

			}
		}
		else if (gameState == GameState::BATTLE)
		{
			if (IsMusicStreamPlaying(playingMusic))
			{
				StopMusicStream(playingMusic);
			}
			playMusic(battleMusic);
			if (isBattleStarted)
			{
				numEnemies = 0;
				Enemy currentEnemy{};
				for (numEnemies = 0; numEnemies < enemies.size(); ++numEnemies)
					if ((int)enemies[numEnemies].getPosition().x == (int)(player.getPosition().x + 0.5) && (int)enemies[numEnemies].getPosition().y == (int)(player.getPosition().y + 0.5))
					{
						currentEnemy = enemies[numEnemies];
						foundEnemy = true;
						break;
					}
				std::vector<Pokemon*> p = currentEnemy.getPokemons();
				if (foundEnemy)
					controller.SetPokemons(player.getPokemons(), p);
				else
					TraceLog(LOG_ERROR, "Enemy not found!");
				isBattleStarted = false;
			}
			if (foundEnemy)
				controller.StartBattlePro();
			EndBattle end = controller.GetConclusion();
			if (end == EndBattle::WIN)
			{
				TraceLog(LOG_INFO, "WIN");
				foundEnemy = false;
				StopMusicStream(battleMusic);
				for (auto& el : player.getPokemons())
				{
					el->upgradeAttack();
					el->upgradeDefense();
				}
				if (board.DeleteEnemy(static_cast<int>(player.getPosition().x + 0.5), static_cast<int>(player.getPosition().y + 0.5)))
				{
					delete drawable[2 + numEnemies];
					drawable.erase(drawable.begin() + 2 + numEnemies);

					enemies[numEnemies].deletePokemons();
					enemies.erase(enemies.begin() + numEnemies);
					if (enemies.empty())
						gameState = GameState::VICTORY;
				}
				isBattleStarted = true;
			}
			else if (end == EndBattle::LOSE)
			{
				foundEnemy = false;
				if (board.DeleteEnemy(static_cast<int>(player.getPosition().x + 0.5), static_cast<int>(player.getPosition().y + 0.5)))
				{
					enemies.erase(enemies.begin() + numEnemies);
				}
				//gameState = GameState::LOSE;
			}
		}
		else if (gameState == GameState::LOSE)
		{
			playMusic(loseMusic);
			DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(BLACK, 0.5f));
			ClearBackground(RED);
			DrawTextEx(font, "You lose!!!", { SCREEN_WIDTH / 2 - MeasureTextEx(font, "You loose!!!", 50, 2).x / 2, 200 }, 50, 2, GREEN);

			Rectangle rectB = { SCREEN_WIDTH / 2 - 150, 300, 300, 80 };

			DrawTexturePro(
				restBut,
				{ 0, 0, (float)restBut.width, (float)restBut.height },
				rectB,
				{ 0,0 },
				0.0f,
				WHITE
			);
			Rectangle extB = { SCREEN_WIDTH / 2 - 150, 400, 300, 80 };
			DrawTexturePro(
				extBut,
				{ 0, 0, (float)extBut.width, (float)extBut.height },
				extB,
				{ 0,0 },
				0.0f,
				WHITE
			);

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Vector2 mousePos = GetMousePosition();
				if (CheckCollisionPointRec(mousePos, rectB))
				{
					gameState = GameState::RESTART;
					isSoundWin = false;
				}
				else if (CheckCollisionPointRec(mousePos, extB))
				{
					gameState = GameState::START;
					isSave = false;
					StopMusicStream(startMusic);
					isBattleStarted = true;
					StopMusicStream(battleMusic);
					drawable.clear();
					enemies.clear();
				}

			}
		}
		else if (gameState == GameState::RESTART)
		{
			isSoundWin = false;
			isBattleStarted = true;
			foundEnemy = false;

			drawable.clear();
			drawable.push_back(&board);
			drawable.push_back(&player);

			player.setStartPosition();
			board.Restart(startFile);

			enemies.clear();
			createEnemies(enemies, board, pokemons);
			createDrawable(drawable, enemies);

			gameState = GameState::PLAYING;

			StopMusicStream(battleMusic);
			StopMusicStream(loseMusic);
			for (auto& el : player.getPokemons())
				el->setHealthPoint(el->getHealthPoint());
		}

		EndDrawing();
	}
	board.writeSave(player.getPosition().x + 0.5, player.getPosition().y + 0.5);
	if(isPlaying)
		saveInfo(enemies, player.getPokemons());

	//очистка
	UnloadFont(font);

	// Закрытие
	CloseAudioDevice();
	CloseWindow();
	for (int i{}; i < pokemons.size(); ++i)
		delete pokemons[i];
	pokemons.clear();
	drawable.clear();
	enemies.clear();
}
