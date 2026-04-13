#include "Paddle.h"

Paddle::Paddle(float x, float y, float width, float height)
    : x(x), y(y), originalWidth(width), currentWidth(width), height(height), effectRemainingTime(0.0f) {}

void Paddle::MoveLeft(float speed) {
    x -= speed;
    if (x < 5) x = 5; // 左边界
}

void Paddle::MoveRight(float speed) {
    x += speed;
    if (x + currentWidth > GetScreenWidth() - 5) x = GetScreenWidth() - 5 - currentWidth; // 右边界
}

void Paddle::Update(float dt) {
    // 处理加长效果倒计时
    if (effectRemainingTime > 0) {
        effectRemainingTime -= dt;
        if (effectRemainingTime <= 0) {
            // 到期恢复原始宽度
            currentWidth = originalWidth;
            // 调整x位置，保持板居中
            x = x + (currentWidth - originalWidth) / 2;
        }
    }
}

void Paddle::Extend(float extraWidth, float duration) {
    if (effectRemainingTime <= 0) {
        // 首次加长：记录原始宽度，调整x保持居中
        float centerX = x + currentWidth / 2;
        currentWidth = originalWidth + extraWidth;
        x = centerX - currentWidth / 2;
    } else {
        // 已有效果：刷新时间，不重复加长
        currentWidth = originalWidth + extraWidth;
    }
    effectRemainingTime = duration; // 重置倒计时
}

void Paddle::Reset() {
    x = 340.0f;
    y = 550.0f;
    currentWidth = originalWidth;
    effectRemainingTime = 0.0f;
}

Rectangle Paddle::GetRect() const {
    return {x, y, currentWidth, height};
}

void Paddle::Draw() const {
    DrawRectangleRec(GetRect(), BLUE);
    DrawRectangleLinesEx(GetRect(), 2, WHITE);
}