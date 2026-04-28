#pragma once
#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "Config.h"
#include <vector>
#include <enet/enet.h>
#include "Network.h"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    VICTORY
};

struct Particle {
    Vector2 pos;
    Vector2 vel;
    Color color;
    float life;
};

// 同步状态结构体
struct GameSyncState {
    float ballX, ballY;
    float ballSpeedX, ballSpeedY;
    float paddle1X;
    float paddle2X;
    int score;
    int lives;
    bool brickActive[40];
};

class Game {
public:
    Game(bool isHost_, Network& net_);
    ~Game();
    void Run();

    void SerializeState(unsigned char* buffer);
    void DeserializeState(unsigned char* buffer);

private:
    void Init();
    void Update();
    void Draw();
    void Reset();

    GameState currentState;
    Ball ball;
    Paddle paddle1;
    Paddle paddle2;
    std::vector<Brick> bricks;
    std::vector<Particle> particles;

    int score;
    int lives;

public:
    bool isHost;
    Network& net;
    bool keyLeft, keyRight;

    // ===================== 【步骤5：插值平滑变量】 =====================
    GameSyncState prevState;
    GameSyncState targetState;
    float interpTimer;
};