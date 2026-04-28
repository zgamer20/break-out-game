#ifndef BALL_H
#define BALL_H
#include <raylib.h>

class Ball
{
private:
    Vector2 position;
    Vector2 speed;
    float radius;

public:
    Ball(Vector2 pos, Vector2 spd, float r);
    void Move();
    void BounceEdge(int screenWidth, int screenHeight);
    void UpdateSpeed(float dt);
    void ReverseY();
    void SlowDown(float scale, float time); // 存在

    Vector2 GetPos() const;
    void Draw() const;
    bool CheckBrickCollision(Rectangle brick);
};
#endif