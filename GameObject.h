#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "raylib.h"

// 顶层基类
class GameObject {
public:
    virtual ~GameObject() = default;
};

// 物理类：位置、速度、碰撞
class PhysicalObject : public GameObject {
public:
    Vector2 position{};
    Vector2 speed{};
    float radius = 0.0f;
};

// 视觉类：颜色、可见
class VisualObject : public GameObject {
public:
    Color color = WHITE;
    bool visible = true;
};

#endif