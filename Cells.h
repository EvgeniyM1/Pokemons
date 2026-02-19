#pragma once
#include "raylib.h"
#include "string"
#include "Pokemon.h"
#include "Button.h"
#include "Counter.h"

class PokemonsCells : public Subject, public Observer
{
    Rectangle cell;
    Pokemon* pokemon;
    Color color;
    Color defaultColor;
    bool isSelected;
    Color selectedColor;
    Button button;
    bool isMaxPokemon;

    void DrawStat();

    void onClick();

public:
    PokemonsCells(float x, float y, float w, float h, Pokemon* p, Color c);

    void Draw();

    Rectangle getRectangle();
    bool getSelected();
    Button getButton();
    Pokemon* getPokemon();

    void setColor(Color c);
    void setDefaultColor();
    void select();
    void clearSelect();
    void setPosition(float x, float y);

    void showInfo();

    void update(float, float)override;
};