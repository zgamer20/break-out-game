#pragma once
#include "raylib.h"

class Paddle {
private:
    float x, y;
    float originalWidth;  // 原始宽度
    float currentWidth;   // 当前宽度
    float height;
    float effectRemainingTime; // 效果剩余时间

public:
    Paddle(float x, float y, float width, float height);
    void MoveLeft(float speed);
    void MoveRight(float speed);
    void Update(float dt); // 处理效果倒计时
    void Extend(float extraWidth, float duration); // 加长板
    void Reset(); // 重置板
    Rectangle GetRect() const;
    void Draw() const;
    float GetEffectRemainingTime() const { return effectRemainingTime; }
};