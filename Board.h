#pragma once
#include "GameState.h"
#include "Resources.h"
#include "Drawer.h"
#include "Potion.h"
#include "Observable.h"
#include <fstream>

enum class Cells { NONE, WALL, ENEMY, POTION, PLAYER};

class Board : public Drawable, public Observer
{
	std::vector<Vector2> walls = { {2,2}, {4,5} };
	std::vector<Vector2> enemies = { {3,1}, {5, 3}, {4, 4} };
	std::vector<Vector2> potionsPosition = { {3,2}, {6, 4}, {5, 5} };
	Cells** map;
	std::string fileForSave;

	Vector2 startPositionPlayer;

	std::vector<HealthPotion*> potions;

	void DrawObject(int x, int y, int size, Texture texture)
	{
		DrawTexturePro(
			texture,
			{ 0, 0, (float)texture.width, (float)texture.height },
							{
								(float)x * size,
								(float)y * size,
								(float)size,
								(float)size
							},
			{ 0,0 }, 0.0f, WHITE);
	}
public:
	~Board();
	Board(std::string);

	void Restart(std::string);

	HealthPotion* getPotion(float x, float y);

	Vector2 getStartPosition();
	std::string getFileForSave();

	Cells isObject(int x, int y) const;

	void Draw() override;

	bool DeleteEnemy(int x, int y);
	bool DeletePotion(int x, int y);

	void startBattle();
	void update(float x, float y) override;

	bool readSave(std::string);

	void writeSave(int, int);
};
