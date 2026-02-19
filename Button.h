#pragma once
#include "raylib.h"
#include "string"
#include "GameState.h"

class Button
{
    std::string text;
    Rectangle background;
    Vector2 position;
    int fontSize;
    Color colorButton;

public:
    Button(std::string str, int w, int h, Vector2 pos, int size, Color color);

    void Draw();

    void Draw(GameState gm);
    void OnClick(GameState gm);

    Rectangle getRectangle() const;
};