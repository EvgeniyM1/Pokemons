#include "Cells.h"

void PokemonsCells::DrawStat()
{
    DrawText(("Name: " + pokemon->getName()).c_str(), 300, 10, 20, BLACK);
    DrawText(("HP: " + std::to_string(pokemon->getHealthPoint())).c_str(), 300, 40, 20, BLACK);
    DrawText(("DEF: " + std::to_string(pokemon->getDefense())).c_str(), 300, 70, 20, BLACK);
    DrawText(("ATK: " + std::to_string(pokemon->getAttack())).c_str(), 300, 100, 20, BLACK);
    DrawText(("TYPE: " + pokemon->getStrType()).c_str(), 300, 130, 20, BLACK);
}

void PokemonsCells::onClick()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, button.getRectangle()))
        {
            if (isSelected == false)
            {
                Invoke(1.F);
                //if(!isMaxPokemon)
                //    select();
            }
            else if (isSelected == true)
            {
                Invoke(0.F);
                clearSelect();
            }
        }
    }
}

PokemonsCells::PokemonsCells(float x, float y, float w, float h, Pokemon* p, Color c) :
    cell{}, pokemon{ p }, defaultColor(c), button{ "Select", 100, 40, {700.F, 150.F}, 20, GREEN }
{
    cell.x = x;
    cell.y = y;
    cell.width = w;
    cell.height = h;
    color = defaultColor;
    isSelected = false;
    isMaxPokemon = false;
    selectedColor = GREEN;
}

void PokemonsCells::Draw()
{
    Texture texture = pokemon->getTexture();
    Color currentColor = isSelected ? selectedColor : color;

    DrawRectangleRec(cell, currentColor);
    DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, cell, {}, 0.0, WHITE);
}

Rectangle PokemonsCells::getRectangle()
{
    return cell;
}

bool PokemonsCells::getSelected()
{
    return isSelected;
}

Button PokemonsCells::getButton()
{
    return button;
}

Pokemon* PokemonsCells::getPokemon()
{
    return pokemon;
}

void PokemonsCells::setColor(Color c)
{
    color = c;
}

void PokemonsCells::setDefaultColor()
{
    color = defaultColor;
}

void PokemonsCells::select()
{
    isSelected = true;
}

void PokemonsCells::clearSelect()
{
    isSelected = false;
}

void PokemonsCells::setPosition(float x, float y)
{
    cell.x = x;
    cell.y = y;
}

void PokemonsCells::showInfo()
{
    Texture texture = pokemon->getTexture();
    //Button button{ "Select", 100, 40, {700.F, 150.F}, 20, GREEN};

    DrawRectangle(0, 0, 800, 200, RED);
    DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, { 10.F, 10.F, 100.F, 100.F }, {}, 0.0, WHITE);
    DrawStat();
    if (isSelected) DrawText("Selected", 300, 160, 20, BLACK);
    button.Draw(gameState);

    onClick();
}

void PokemonsCells::update(float count, float)
{
    if ((int)count <= 3)
        select();
    else
    {
        Invoke(0.F);
    }
}

