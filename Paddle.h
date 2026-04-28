#ifndef PADDLE_H
#define PADDLE_H

#include <raylib.h>

class Paddle {
private:
    Rectangle rect;
    float speed;

public:
    Paddle(float x, float y, float w, float h);
    void MoveLeft(int step);
    void MoveRight(int step);
    void Update(float dt);
    void Extend(float dw, float duration);
    Rectangle GetRect();
    void Draw();
};

#endif