#pragma once
#include<raylib.h>
enum class GameState { START, CONTINUE, CHANGING_POKEMONS, PREVIEW, PLAYING, VICTORY, BATTLE, LOSE, RESTART, PAUSE};
static inline const int SCREEN_WIDTH = 800;
static inline const int SCREEN_HEIGHT = 800;
static inline const Color FORBIDDEN_COLOR = { 150, 50, 50, 255 };
static inline const float eps = 0.2F;
static inline const int SIZE = 8;

extern inline GameState gameState = GameState::PLAYING;


static float myLerp(float start, float end, float t)
{
	//ограничение по t
	t = (t < 0) ? 0 : (t > 1) ? 1 : t;
	return start + (end - start) * t;
}