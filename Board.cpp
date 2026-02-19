#include "Board.h"

Board::~Board()
{
	for (int i{}; i < SIZE; ++i)
	{
		delete[] map[i];
		map[i] = nullptr;
	}
	delete[] map;
	map = nullptr;
	for (auto& el : potions)
	{
		delete el;
		el = nullptr;
	}
	potions.clear();
}
Board::Board(std::string str)
{
	fileForSave = str;
	map = new Cells*();
	startPositionPlayer = {};
}

void Board::Restart(std::string file)
{
	{
		readSave(file);
		if (map != nullptr)
		{
			delete[] map;
			map = nullptr;
		}
		for (auto& el : potions)
		{
			delete el;
			el = nullptr;
		}
		potions.clear();
		map = new Cells * [SIZE];
		for (int i{}; i < SIZE; ++i)
			map[i] = new Cells[SIZE](Cells::NONE);
		for (auto& el : walls)
			map[static_cast<int>(el.y)][static_cast<int>(el.x)] = Cells::WALL;
		for (auto& el : enemies)
			map[static_cast<int>(el.y)][static_cast<int>(el.x)] = Cells::ENEMY;
		for (auto& el : potionsPosition)
		{
			map[static_cast<int>(el.y)][static_cast<int>(el.x)] = Cells::POTION;
			HealthPotion* temp = new HealthPotion({ el.x, el.y }, ResourceManager::GetTexture("potion"), 35);
			potions.push_back(temp);
		}
	}
}

HealthPotion* Board::getPotion(float x, float y)
{
	for (auto& el : potions)
		if ((int)(el->getPosition().x) == (int)x && (int)(el->getPosition().y) == (int)y)
			return el;
	return nullptr;
}

Vector2 Board::getStartPosition()
{
	return startPositionPlayer;
}

std::string Board::getFileForSave()
{
	return fileForSave;
}

Cells Board::isObject(int x, int y) const
{
	return map[y][x];
}

void Board::Draw()
{
	const int cellSize = SCREEN_WIDTH / SIZE;
	Texture obstacle = ResourceManager::GetTexture("obstacle");
	Texture grass = ResourceManager::GetTexture("grass");
	for (int row{}; row < SIZE; ++row)
		for (int col{}; col < SIZE; ++col)
		{
			//определяем цвет клетки
			Color color = ((row + col) % 2 == 0) ? LIGHTGRAY : DARKBLUE;

			//изменяем цвет для клекти
			if (isObject(col, row) == Cells::WALL)
				color = FORBIDDEN_COLOR;
			else if (isObject(col, row) == Cells::ENEMY)
				color = YELLOW;

			//
			DrawRectangle(col * cellSize, row * cellSize, cellSize, cellSize, color);
			DrawObject(col, row, cellSize, grass);

			if (isObject(col, row) == Cells::WALL)
			{
				DrawObject(col, row, cellSize, obstacle);
			}
			for (auto& el : potions)
				el->Draw();
		}
	//for (int i = 0; i <= SIZE; ++i)
	//{
	//	DrawLine(i * cellSize, 0, i * cellSize, SCREEN_HEIGHT, BLACK);
	//	DrawLine(0, i * cellSize, SCREEN_WIDTH, i * cellSize, BLACK);
	//}
}

bool Board::DeleteEnemy(int x, int y)
{
	if (map[y][x] == Cells::ENEMY)
	{
		map[y][x] = Cells::NONE;
		return true;
	}
	return false;
}

bool Board::DeletePotion(int x, int y)
{
	if (map[y][x] == Cells::POTION)
	{
		map[y][x] = Cells::NONE;
		return true;
	}
	return false;
}

void Board::startBattle()
{
	gameState = GameState::BATTLE;
}

void Board::update(float x, float y)
{
	for(int i{}; i < potions.size(); ++i)
		if ((int)potions[i]->getPosition().x == (int)x && (int)potions[i]->getPosition().y == (int)y)
		{
			potions.erase(potions.begin() + i);
			DeletePotion((int)x, (int)y);
		}
	//TraceLog(LOG_INFO, "Invoke");
}

bool Board::readSave(std::string fileName)
{
	std::vector<Vector2> tempEnemies;
	std::vector<Vector2> tempWalls;
	std::vector<Vector2> tempPotions;

	std::ifstream file(fileName);
	if (!file.is_open()) return false;

	int i = 0;
	std::string str;
	bool isPlayer = false;

	while (std::getline(file, str) && i < SIZE)
	{
		int len = (int)str.length();
		if (len != SIZE) return false;

		for (int j{}; j < len; ++j)
		{
			switch (str[j])
			{
			case 'P':
				if (!isPlayer)
				{
					startPositionPlayer = { (float)j, (float)i };
					isPlayer = false;
				}
				break;

			case '*':
				tempEnemies.push_back({(float)j, (float)i});
				break;

			case '#':
				tempWalls.push_back({ (float)j, (float)i });
				break;
				
			case '$':
				tempPotions.push_back({ (float)j, (float)i });
				break;

			//default:
			//	break;
			}
		}

		++i;
	}
	if (i != SIZE) return false;

	enemies.assign(tempEnemies.begin(), tempEnemies.end());
	walls.assign(tempWalls.begin(), tempWalls.end());
	potionsPosition.assign(tempPotions.begin(), tempPotions.end());

	tempEnemies.clear();
	tempWalls.clear();
	tempPotions.clear();
	file.close();
	return true;
}

void Board::writeSave(int posX, int posY)
{
	Cells c;
	if (posX < SIZE && posY < SIZE)
	{
		c = map[posY][posX];
		map[posY][posX] = Cells::PLAYER;
	}
	else return;
	std::ofstream file(fileForSave);
	if (!file.is_open()) return;
	for (int y{}; y < SIZE; ++y)
	{
		for (int x{}; x < SIZE; ++x)
			switch (map[y][x])
			{
			case Cells::NONE:
				file << '.';
				break;
			case Cells::WALL:
				file << "#";
				break;
			case Cells::ENEMY:
				file << "*";
				break;
			case Cells::POTION:
				file << "$";
				break;
			case Cells::PLAYER:
				file << "P";
				break;
			default:
				break;
			}
		file << '\n';
	}
	map[posY][posX] = c;
}
