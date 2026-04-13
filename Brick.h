#include "GameObject.h"
#ifndef BRICK_H
#define BRICK_H
#include "raylib.h"

class Brick : public PhysicalObject, public VisualObject{
private:
    Rectangle rect;
    bool active;
    //Color color;

public:
    Brick(float x, float y, float width, float height, Color c);
    void Draw();
    bool IsActive() { return active; }
    void SetActive(bool a) { active = a; }
    Rectangle GetRect() { return rect; }
    Color GetColor() const { return color; }
};

#endif
//g++ main.cpp PowerUp.cpp Paddle.cpp Ball.cpp Brick.cpp -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11