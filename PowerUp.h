#pragma once

#include <vector>
#include <cmath>
#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"   

// 粒子结构体
struct Particle {
    Vector2 pos;    // 位置
    Vector2 vel;    // 速度
    Color color;    // 颜色
    float life;     // 剩余生命周期（秒）
};

enum class PowerUpType {
    PADDLE_EXTEND,
    MULTI_BALL,
    SLOW_BALL
};

class Game;

class PowerUp {
public:
    Vector2 position;
    PowerUpType type;
    bool active;
    float duration;
    float speed;

    PowerUp(float x, float y, PowerUpType t);
    void Update(float dt);
    void Draw();
    bool CheckPaddleCollision(const Rectangle& paddleRect);
    void Apply(Game& game);
};

class Game {
public:
    std::vector<Ball> balls;
    Paddle paddle;
    std::vector<Brick> bricks;
    std::vector<PowerUp> powerUps;
    std::vector<Particle> particles;  // 新增：粒子列表

    int score;
    int lives;
    bool gameOver;
    bool victory;
    float gameTime;

    float slowBallRemainingTime;
    float originalBallSpeed;

    Game();
    void Reset();
    void Update(float dt);
    void Draw();
};