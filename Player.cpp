#include "Player.h"

#pragma once

const int cellSize = SCREEN_WIDTH / SIZE;

Vector2 Player::SetNewPosition(Board& board)
{
	Vector2 newTargetPosition = newPosition;
	bool isLeftUpper = board.isObject(static_cast<int>(newPosition.x + eps), static_cast<int>(newPosition.y + 2 * eps)) == Cells::WALL,
		isRightUpper = board.isObject(static_cast<int>(newPosition.x + 1 - eps), static_cast<int>(newPosition.y + 2 * eps)) == Cells::WALL,
		isRightLower = board.isObject(static_cast<int>(newPosition.x + 1 - eps), static_cast<int>(newPosition.y + 1 - eps)) == Cells::WALL,
		isLeftLower = board.isObject(static_cast<int>(newPosition.x + eps), static_cast<int>(newPosition.y + 1 - eps)) == Cells::WALL,
		isForward = board.isObject(static_cast<int>(newPosition.x + 0.5), static_cast<int>(newPosition.y + 1 - 0.5 * eps)) == Cells::WALL,
		isBack = board.isObject(static_cast<int>(newPosition.x + 0.5), static_cast<int>(newPosition.y + eps)) == Cells::WALL,
		isRight = board.isObject(static_cast<int>(newPosition.x + 1 - 0.5 * eps), static_cast<int>(newPosition.y + 0.6)) == Cells::WALL,
		isLeft = board.isObject(static_cast<int>(newPosition.x + 0.5F * eps), static_cast<int>(newPosition.y + 0.6)) == Cells::WALL;
	if (IsKeyDown(KEY_RIGHT))
	{
		if(newPosition.x < SIZE - 1 && !isRightLower && !isRightUpper && !isRight)
			newTargetPosition.x += 1 * moveSpeed * GetFrameTime();
		lastKeyInt = KEY_RIGHT;
		lastKey = "right";
	}
	if (IsKeyDown(KEY_LEFT))
	{
		if(newPosition.x > 0 && !isLeftLower && !isLeftUpper && !isLeft)
			newTargetPosition.x -= 1 * moveSpeed * GetFrameTime();
		lastKeyInt = KEY_LEFT;
		lastKey = "left";

	}
	if (IsKeyDown(KEY_UP))
	{
		if(newPosition.y > 0 && !isRightUpper && !isLeftUpper && !isBack)
			newTargetPosition.y -= 1 * moveSpeed * GetFrameTime();
		lastKeyInt = KEY_UP;
		lastKey = "top";

	}
	if (IsKeyDown(KEY_DOWN))
	{
		if(newPosition.y < SIZE - 1 && !isRightLower && !isLeftLower && !isForward)
			newTargetPosition.y += 1 * moveSpeed * GetFrameTime();
		lastKeyInt = KEY_DOWN;
		lastKey = "bottom";
	}
	return newTargetPosition;
}

Player::Player(std::string str) :
	texture(ResourceManager::GetTexture(str)), position({ 0.F, 0.F }), newPosition({}), animator{ 0, 3, 0, 0.1F }
{
	//std::string str;
	textures["right"] = ResourceManager::GetTexture("player-right");
	textures["left"] = ResourceManager::GetTexture("player-left");
	textures["top"] = ResourceManager::GetTexture("player-top");
	textures["bottom"] = ResourceManager::GetTexture("player");

	lastKey = "bottom";
	lastKeyInt = KEY_DOWN;

	for (int i{}; i <= 3; ++i)
	{
		if (i == 0) str = "player";
		else str = "player-" + std::to_string(i);
		Texture temp = ResourceManager::GetTexture(str);
		bottomDirection.push_back(temp);
	}

	for (int i{}; i <= 3; ++i)
	{
		if (i == 0) str = "player-right";
		else str = "player-right-" + std::to_string(i);
		Texture temp = ResourceManager::GetTexture(str);
		rightDirection.push_back(temp);
	}

	for (int i{}; i <= 3; ++i)
	{
		if (i == 0) str = "player-left";
		else str = "player-left-" + std::to_string(i);
		Texture temp = ResourceManager::GetTexture(str);
		leftDirection.push_back(temp);
	}

	for (int i{}; i <= 3; ++i)
	{
		if (i == 0) str = "player-top";
		else str = "player-top-" + std::to_string(i);
		Texture temp = ResourceManager::GetTexture(str);
		topDirection.push_back(temp);
	}
}

Vector2 Player::getPosition()
{
	return position;
}

std::vector<Pokemon*>& Player::getPokemons()
{
	return pokemons;
}

void Player::Move(Board& board)
{
	bool moved = false;
	Vector2 newTargetPosition = SetNewPosition(board);
	bool playerAction = newTargetPosition.x != position.x || newTargetPosition.y != position.y;
	if (playerAction)
	{
		newPosition = newTargetPosition;
		moved = true;
	}

	position.x = myLerp(position.x, newPosition.x, 1.F);
	position.y = myLerp(position.y, newPosition.y, 1.F);
	if (board.isObject(static_cast<int>(position.x + 0.5), static_cast<int>(position.y + 0.5)) == Cells::ENEMY)
	{
		TraceLog(LOG_INFO, "Battle Strted!!");
		board.startBattle();
	}
	if (board.isObject(static_cast<int>(position.x + 0.5), static_cast<int>(position.y + 0.5)) == Cells::POTION)
	{
		TraceLog(LOG_INFO, "Get Heal");
		PlaySound(ResourceManager::GetSound("move"));
		int heal = (int)board.getPotion(position.x + 0.5F, position.y + 0.5F)->Applay();
		for (auto& el : pokemons)
			el->setHealthPoint(el->getHealthPointNow() + heal);
		Invoke(position.x + 0.5F, position.y + 0.5F);
	}
}

void Player::Draw()
{
	if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
		switch (lastKeyInt)
		{
		case KEY_DOWN:
			animator.animationUpdate(bottomDirection, 100.F, 100.F, position.x, position.y);
			break;
		case KEY_UP:
			animator.animationUpdate(topDirection, 100.F, 100.F, position.x, position.y);
			break;
		case KEY_LEFT:
			animator.animationUpdate(leftDirection, 100.F, 100.F, position.x, position.y);
			break;
		case KEY_RIGHT:
			animator.animationUpdate(rightDirection, 100.F, 100.F, position.x, position.y);
			break;
		}
	else
	{
		texture = textures[lastKey];
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
}

void Player::setStartPosition()
{
	newPosition = { 0.F, 0.F };
	position = { 0.F, 0.F };
}

void Player::setStartPosition(float x, float y)
{
	newPosition = { x, y };
	position = { x, y };
}

void Player::addPokemon(Pokemon* pokemon)
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

void Player::clearPokemons()
{
	for (auto& el : pokemons)
		delete el;
	pokemons.clear();
}
