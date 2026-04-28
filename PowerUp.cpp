#include "PowerUp.h"

PowerUp::PowerUp(float x, float y, PowerUpType t) {
    rect = {x, y, 30, 30};
    type = t;
    active = true;
    speed = 2.0f;
}

void PowerUp::Update(float dt) {
    rect.y += speed;
}

void PowerUp::Draw() {
    if (!active) return;
    Color c;
    switch(type) {
        case PowerUpType::PADDLE_EXTEND: c = PURPLE; break;
        case PowerUpType::MULTI_BALL: c = ORANGE; break;
        case PowerUpType::SLOW_BALL: c = SKYBLUE; break;
    }
    DrawRectangleRec(rect, c);
}

bool PowerUp::IsOutOfScreen(int h) {
    return rect.y > h;
}