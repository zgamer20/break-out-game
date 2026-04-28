#include "Brick.h"

Brick::Brick(float x, float y, float w, float h) {
    rect = {x, y, w, h};
    active = true;
}

void Brick::Draw() {
    if (active)
        DrawRectangleRec(rect, GREEN);
}

bool Brick::IsActive() { return active; }
void Brick::SetActive(bool a) { active = a; }
Rectangle Brick::GetRect() { return rect; }