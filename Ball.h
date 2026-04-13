#pragma once
#include "raylib.h"

class Ball {
private:
    Vector2 position;
    Vector2 direction;
    float radius;
    float speed;
    bool launched;

public:
    Ball(Vector2 pos, Vector2 dir, float r);
    void SetPosition(Vector2 pos) { position = pos; }
    void SetDirection(Vector2 dir) { direction = dir; }
    void SetSpeed(float s) { speed = s; }
    Vector2 GetPosition() const { return position; }
    bool IsLaunched() const { return launched; }
    void ResetToPaddle(float paddleX, float paddleY);
    void Launch(float paddleX, float paddleWidth);
    void ApplyGravity();
    void Move();
    void BounceEdge(int screenWidth, int screenHeight);
    void BouncePaddle(const Rectangle& paddleRect);
    bool CheckBrickCollision(const Rectangle& brickRect);
    void Draw() const;
};