#include "Animation.h"

Animation::Animation(int first, int last, int curr, float s) :
    firstFrame(first), lastFrame(last), currentFrame(curr), speed(s)
{
    durationLeft = s;
}

void Animation::animationUpdate(std::vector<Texture> object, float width, float height, float x, float y)
{
    float dt = GetFrameTime();
    durationLeft -= dt;
    if (durationLeft <= 0)
    {
        durationLeft = speed;
        currentFrame++;
    }
    if (currentFrame > lastFrame)
        currentFrame = firstFrame;
    DrawTexturePro(object[currentFrame], { 0, 0, (float)object[currentFrame].width, (float)object[currentFrame].height }, { x * width, y * height, width, height }, { 0, 0 }, 0.0F, WHITE);
}
