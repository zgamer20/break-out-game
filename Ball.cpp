#include "Ball.h"
#include "raylib.h"
#include <cmath>

Ball::Ball(Vector2 pos, Vector2 dir, float r)
    : position(pos), direction(dir), radius(r), speed(300.0f), launched(false) {}

void Ball::ResetToPaddle(float paddleX, float paddleY) {
    position.x = paddleX;
    position.y = paddleY - radius - 5;
    direction = { 0.0f, 0.0f };
    launched = false;
}

void Ball::Launch(float paddleX, float paddleWidth) {
    launched = true;
    float hitOffset = (position.x - paddleX) / (paddleWidth / 2.0f);
    float angle = hitOffset * 60.0f * DEG2RAD;
    direction = { sinf(angle), -cosf(angle) };
}

void Ball::ApplyGravity() {
    direction.y += 0.1f * GetFrameTime();
}

void Ball::Move() {
    if (!launched) return;
    position.x += direction.x * speed * GetFrameTime();
    position.y += direction.y * speed * GetFrameTime();
}

void Ball::BounceEdge(int screenWidth, int screenHeight) {
    if (!launched) return;
    if (position.x - radius <= 5 || position.x + radius >= screenWidth - 5)
        direction.x *= -1;
    if (position.y - radius <= 5)
        direction.y *= -1;
}

void Ball::BouncePaddle(const Rectangle& paddleRect) {
    if (!launched) return;
    Rectangle ballRect = { position.x - radius, position.y - radius, radius*2, radius*2 };
    if (CheckCollisionRecs(ballRect, paddleRect)) {
        float hitOffset = (position.x - (paddleRect.x + paddleRect.width/2)) / (paddleRect.width/2.0f);
        float angle = hitOffset * 60.0f * DEG2RAD;

        // ✅✅✅ 这行我已经帮你彻底修好 ✅✅✅
        float dx = sinf(angle);
        float dy = -fabsf(cosf(angle));
        direction = { dx, dy };

        position.y = paddleRect.y - radius - 1;
    }
}

bool Ball::CheckBrickCollision(const Rectangle& brickRect) {
    if (!launched) return false;
    Rectangle br = { position.x - radius, position.y - radius, radius*2, radius*2 };
    if (CheckCollisionRecs(br, brickRect)) {
        float dx = position.x - (brickRect.x + brickRect.width/2);
        float dy = position.y - (brickRect.y + brickRect.height/2);
        if (fabs(dx) > fabs(dy)) direction.x *= -1;
        else direction.y *= -1;
        return true;
    }
    return false;
}

void Ball::Draw() const {
    DrawCircleV(position, radius, YELLOW);
    DrawCircleLinesV(position, radius, WHITE);
}