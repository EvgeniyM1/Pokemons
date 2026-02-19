#include "Button.h"

Button::Button(std::string str, int w, int h, Vector2 pos, int size, Color color) :
    text(str), background(), position(pos), fontSize(size), colorButton(color)
{
    background.width = w;
    background.height = h;
    background.x = pos.x;
    background.y = pos.y;
}

void Button::Draw()
{
    DrawRectangleRec(background, colorButton);
    DrawText(text.c_str(), (int)(position.x) + (int)(background.width / 2) - text.length() * fontSize / 3, (int)position.y + background.height / 3, fontSize, BLACK);
    Vector2 mousePos = GetMousePosition();
}

void Button::Draw(GameState gm)
{
    DrawRectangleRec(background, colorButton);
    DrawText(text.c_str(), (int)(position.x) + (int)(background.width / 2) - text.length() * fontSize / 3, (int)position.y + background.height / 3, fontSize, BLACK);
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mousePos, background))
        {
            OnClick(gm);
        }
    }
}

void Button::OnClick(GameState gm)
{
    gameState = gm;
}

Rectangle Button::getRectangle() const
{
    return background;
}
