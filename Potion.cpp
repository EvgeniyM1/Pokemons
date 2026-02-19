#include "Potion.h"
#include "GameState.h"

Potion::Potion(Vector2 pos, Texture tex) :
	position(pos), texture(tex)
{}

void Potion::Draw()
{
	int cellSize = SCREEN_WIDTH / SIZE;
	DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, { position.x * cellSize, position.y * cellSize, SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8 }, {}, 0.0, WHITE);
}

HealthPotion::HealthPotion(Vector2 pos, Texture tex, int health) :
	Potion(pos, tex), healAmout(health)
{
	
}

float HealthPotion::Applay()
{
	return (float)healAmout;
}

Vector2 HealthPotion::getPosition()
{
	return position;
}
