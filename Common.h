#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <vector>

// 动作枚举（Input模块用）
enum class Action {
    NONE,
    LEFT,
    RIGHT,
    JUMP,
    PAUSE,
    SHADOW
};

// 实体类型枚举
enum class EntityType {
    PLAYER,
    SHADOW,
    PLATFORM,
    ENEMY,
    MECHANIC,
    NONE
};

// 基础实体结构体（所有实体的父类）
struct Entity {
    EntityType type = EntityType::NONE;
    Vector2 position = {0, 0};
    Vector2 size = {0, 0};
    bool active = true;
};

// 玩家结构体（继承Entity）
struct Player : Entity {
    bool onGround = false;
    bool canJump = true;
};

// 影子结构体（继承Entity）
struct Shadow : Entity {
    float lifetime = 0.0f; // 剩余存活时间（秒）
};

// 全局重力常量（所有模块共享）
extern float GRAVITY;

#endif // COMMON_H