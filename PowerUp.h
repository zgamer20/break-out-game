#ifndef POWERUP_H
#define POWERUP_H

#include <raylib.h>

enum class PowerUpType {
    PADDLE_EXTEND,
    MULTI_BALL,
    SLOW_BALL
};

class PowerUp {
public:
    Rectangle rect;
    PowerUpType type;
    bool active;
    float speed;

    PowerUp(float x, float y, PowerUpType t);
    void Update(float dt);
    void Draw();
    bool IsOutOfScreen(int h);
};

#endif