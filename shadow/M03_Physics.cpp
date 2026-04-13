#include "M03_Physics.h"
#include "Common.h"

// 定义全局重力常量
float GRAVITY = 800.0f;

bool CheckCollision(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2) {
    return (pos1.x < pos2.x + size2.x &&
            pos1.x + size1.x > pos2.x &&
            pos1.y < pos2.y + size2.y &&
            pos1.y + size1.y > pos2.y);
}

void MoveEntity(Entity& e, Vector2 delta, const std::vector<Entity*>& statics) {
    e.position.x += delta.x;
    e.position.y += delta.y;
}

void UpdatePhysics(float dt) {
    // 基础版本留空
}