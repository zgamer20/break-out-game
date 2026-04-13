#ifndef M03_PHYSICS_H
#define M03_PHYSICS_H

#include "Common.h"
#include <vector>
#include "raylib.h"

// 声明全局重力常量（定义放到.cpp）
extern float GRAVITY;

// 函数声明（实现移到M03_Physics.cpp）
bool CheckCollision(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2);
void MoveEntity(Entity& e, Vector2 delta, const std::vector<Entity*>& statics);
void UpdatePhysics(float dt);

#endif // M03_PHYSICS_H