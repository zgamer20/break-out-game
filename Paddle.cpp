#include "Paddle.h"

Paddle::Paddle(float x, float y, float w, float h) {
    rect = {x, y, w, h};
    speed = 6.0f;
}

void Paddle::MoveLeft(int step) {
    rect.x -= step;
    if (rect.x < 0) rect.x = 0;
}

void Paddle::MoveRight(int step) {
    rect.x += step;
    if (rect.x + rect.width > 800) rect.x = 800 - rect.width;
}

void Paddle::Update(float dt) {
    // 可加减速恢复
}

void Paddle::Extend(float dw, float duration) {
    rect.width += dw;
}

Rectangle Paddle::GetRect() {
    return rect;
}

void Paddle::Draw() {
    DrawRectangleRec(rect, BLUE);
}