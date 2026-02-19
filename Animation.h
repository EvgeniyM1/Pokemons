#pragma once
#include "raylib.h"
#include <vector>

class Animation
{
    int firstFrame;
    int lastFrame;
    int currentFrame;
    float speed;
    float durationLeft;

public:
    Animation(int first, int last, int curr, float s);
    void animationUpdate(std::vector<Texture> object, float width, float height, float x, float y);
};