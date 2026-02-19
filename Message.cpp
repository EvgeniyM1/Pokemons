#include "Message.h"

Message::Message(int sp, int st) :
	key(st), speed(sp)
{
	if (speed <= 0)
		speed = 12;
}

bool Message::WriteMessage(std::string message, StateBattle& stateBattle, const StateBattle& nextState)
{
	if (IsKeyPressed(key))
	{
		stateBattle = nextState;
		return false;
	}
	DrawText(message.c_str(), static_cast<int>(postiton.x), static_cast<int>(postiton.y), 20, BLACK);
	return true;
}


void Message::WriteMessage(std::string message)
{
	DrawText(message.c_str(), static_cast<int>(postiton.x), static_cast<int>(postiton.y), 20, WHITE);
}

void Message::WriteMessage(std::string message, int positionX, int positionY, Color color)
{
	DrawText(message.c_str(), positionX, positionY, 20, color);
}

bool Message::WriteMessagePro(std::string message, StateBattle& stateBattle, const StateBattle& nextState, int& lettersCount)
{
	if (IsKeyPressed(key) && lettersCount == (int)message.length())
	{
		stateBattle = nextState;
		return false;
	}
	else if (IsKeyPressed(key) && lettersCount < (int)message.length())
	{
		lettersCount = (int)message.length();
	}
	//DrawText(TextSubtext(message.c_str(), 0, lettersCount), static_cast<int>(postiton.x), static_cast<int>(postiton.y), 20, WHITE);
	WriteMessage(TextSubtext(message.c_str(), 0, lettersCount));
	return true;
}

int Message::getSpeed()
{
	return speed;
}
