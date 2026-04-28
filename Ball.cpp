#include "Ball.h"
#include <cmath>  // 修复 fabs 错误

Ball::Ball(Vector2 pos, Vector2 spd, float r)
    : position(pos), speed(spd), radius(r)
{}

void Ball::Move()
{
    position.x += speed.x;
    position.y += speed.y;
}

void Ball::BounceEdge(int screenWidth, int screenHeight)
{
    if (position.x - radius <= 0 || position.x + radius >= screenWidth)
        speed.x *= -1;

    if (position.y - radius <= 0)
        speed.y *= -1;
}

void Ball::UpdateSpeed(float dt) {}

void Ball::ReverseY()
{
    speed.y = -fabs(speed.y);
}

//  SlowDown 已实现
void Ball::SlowDown(float scale, float time)
{
    speed.x *= scale;
    speed.y *= scale;
}

Vector2 Ball::GetPos() const
{
    return position;
}

void Ball::Draw() const
{
    DrawCircleV(position, radius, RED);
}

bool Ball::CheckBrickCollision(Rectangle brick)
{
    Rectangle ballRect = { position.x - radius, position.y - radius, radius * 2, radius * 2 };
    if (CheckCollisionRecs(ballRect, brick))
    {
        speed.y *= -1;
        return true;
    }
    return false;
}