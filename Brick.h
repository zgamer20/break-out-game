#ifndef BRICK_H
#define BRICK_H

#include <raylib.h>

class Brick {
private:
    Rectangle rect;
    bool active;

public:
    Brick(float x, float y, float w, float h);
    void Draw();
    bool IsActive();
    void SetActive(bool a);
    Rectangle GetRect();
};

#endif