#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

class Paddle {
private:
    Rectangle rect;
    float screenWidth;

public:
    // 构造函数
    Paddle(float x, float y, float w, float h) {
        rect = { x, y, w, h };
        screenWidth = 800;
    }

    // 左移
    void MoveLeft(float speed) {
        rect.x -= speed;
        if (rect.x < 5) rect.x = 5;
    }

    // 右移
    void MoveRight(float speed) {
        rect.x += speed;
        if (rect.x + rect.width > screenWidth - 5)
            rect.x = screenWidth - rect.width - 5;
    }

    // 绘制
    void Draw() {
        DrawRectangleRec(rect, BLUE);
        DrawRectangleLinesEx(rect, 2, SKYBLUE);
    }

    // 获取矩形
    Rectangle GetRect() {
        return rect;
    }
};

#endif