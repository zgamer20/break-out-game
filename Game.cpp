#include "Game.h"
#include <string>
#include <cstring>

// ===================== 【老师要求：Lerp 函数加在最顶端】 =====================
float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// ===================== 联机版构造函数 =====================
Game::Game(bool isHost_, Network& net_)
    : ball({400, 300}, {0, 0}, 10),
      paddle1(350, 550, 100, 20),
      paddle2(350, 400, 100, 20),
      isHost(isHost_),
      net(net_)
{
    keyLeft = false;
    keyRight = false;
    interpTimer = 0.0f; // 插值计时器初始化

    // 客户端接收数据回调
    if (!isHost) {
        net.SetOnReceive([this](const void* data, size_t len) {
            DeserializeState((unsigned char*)data);
        });
    }
}

Game::~Game() {
}

void Game::Run() {
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "双人联机打砖块"); 
    SetTargetFPS(Config::FPS);

    Init();

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }

    CloseWindow();
}

void Game::Init() {
    score = 0;
    lives = Config::START_LIVES;
    currentState = GameState::PLAYING;

    ball.Reset({400, 300}, {4, -4});
    paddle1.Reset(350, 550);
    paddle2.Reset(350, 400);

    bricks.clear(); 
    for (int row = 0; row < 5; row++) {
        Color currentRowColor = Config::ROW_COLORS[row]; 

        for (int col = 0; col < 8; col++) {
            float x = 50 + col * 90;
            float y = 50 + row * 30;
            bricks.push_back(Brick(x, y, 80, 20, currentRowColor)); 
        }
    }
}

void Game::Reset() {
    Init();
}

// ===================== 核心 Update =====================
void Game::Update() {
    net.Update();

    // 粒子更新（你原来的）
    for (int i = 0; i < particles.size(); i++) {
        particles[i].pos.x += particles[i].vel.x;
        particles[i].pos.y += particles[i].vel.y;
        particles[i].life -= GetFrameTime();

        if (particles[i].life <= 0) {
            particles.erase(particles.begin() + i);
            i--;
        }
    }

    // ===================== 主机：运行游戏逻辑 =====================
    if (isHost) {
        if (IsKeyDown(KEY_LEFT)) paddle1.MoveLeft(6.0f);
        if (IsKeyDown(KEY_RIGHT)) paddle1.MoveRight(6.0f);

        switch (currentState) {
            case GameState::PLAYING:
                ball.Move();
                ball.BounceEdge(GetScreenWidth(), GetScreenHeight());

                if (CheckCollisionCircleRec(ball.GetPosition(), ball.GetRadius(), paddle1.GetRect()))
                    ball.ReverseSpeedY();
                if (CheckCollisionCircleRec(ball.GetPosition(), ball.GetRadius(), paddle2.GetRect()))
                    ball.ReverseSpeedY();

                for (int i = 0; i < bricks.size(); i++) {
                    if (bricks[i].IsActive()) {
                        if (CheckCollisionCircleRec(ball.GetPosition(), ball.GetRadius(), bricks[i].GetRect())) {
                            bricks[i].SetActive(false);
                            ball.ReverseSpeedY();
                            score += Config::BRICK_SCORE;
                            Rectangle brickRect = bricks[i].GetRect();
                            Color brickColor = Config::ROW_COLORS[i / 8];

                            for (int j = 0; j < 10; j++) {
                                Particle p;
                                p.pos = { brickRect.x + rand() % 85, brickRect.y + rand() % 25 };
                                p.vel = { (rand()%100 - 50)/10.0f, (rand()%100 - 50)/10.0f };
                                p.color = brickColor;
                                p.life = 0.5f;
                                particles.push_back(p);
                            }
                        }
                    }
                }

                if (ball.GetPosition().y - ball.GetRadius() > GetScreenHeight()) {
                    lives--; 
                    if (lives <= 0) {
                        currentState = GameState::GAME_OVER;
                    } else {
                        ball.Reset({(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, {4, -4});
                        paddle1.Reset((float)GetScreenWidth() / 2 - 50, GetScreenHeight() - 40);
                        currentState = GameState::PAUSED;
                    }
                }
                break;

            case GameState::PAUSED:
                if (IsKeyPressed(KEY_SPACE)) currentState = GameState::PLAYING;
                break;

            case GameState::GAME_OVER:
                if (IsKeyPressed(KEY_ENTER)) Reset();
                break;

            default: break;
        }

        // 主机发送状态
        unsigned char buffer[256];
        SerializeState(buffer);
        net.SendData(buffer, sizeof(GameSyncState));
    }

    // ===================== 【老师要求：客户端插值平滑】 =====================
    else {
        keyLeft = IsKeyDown(KEY_LEFT);
        keyRight = IsKeyDown(KEY_RIGHT);

        int input = 0;
        if (keyLeft) input |= 1;
        if (keyRight) input |= 2;
        net.SendData(&input, sizeof(input));

        // ===================== 步骤5：插值计时 =====================
        interpTimer += GetFrameTime();
        float t = Clamp(interpTimer / 0.05f, 0.0f, 1.0f);

        // Lerp 平滑球
        float ballX = Lerp(prevState.ballX, targetState.ballX, t);
        float ballY = Lerp(prevState.ballY, targetState.ballY, t);
        ball.SetPosition({ ballX, ballY });

        // Lerp 平滑两个底板
        paddle1.SetX(Lerp(prevState.paddle1X, targetState.paddle1X, t));
        paddle2.SetX(Lerp(prevState.paddle2X, targetState.paddle2X, t));

        // 同步分数、生命、砖块
        score = targetState.score;
        lives = targetState.lives;
        for (int i = 0; i < bricks.size() && i < 40; i++) {
            bricks[i].SetActive(targetState.brickActive[i]);
        }
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (currentState == GameState::PLAYING || currentState == GameState::PAUSED) {
        ball.Draw();
        paddle1.Draw();
        paddle2.Draw();

        for (auto& b : bricks) b.Draw();
        for (auto& p : particles) DrawCircleV(p.pos, 2, p.color);

        DrawText(TextFormat("Score: %d", score), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Lives: %d", lives), GetScreenWidth() - 100, 10, 20, DARKGRAY);

        if (currentState == GameState::PAUSED) {
            DrawText("PRESS SPACE TO PLAY", 250, 300, 20, GRAY);
        }
    }
    else if (currentState == GameState::GAME_OVER) {
        DrawText("GAME OVER", 280, 250, 40, RED);
        DrawText("PRESS ENTER TO RESTART", 240, 320, 20, DARKGRAY);
    }

    EndDrawing();
}

// ===================== 【老师要求：序列化】 =====================
void Game::SerializeState(unsigned char* buffer) {
    GameSyncState s{};
    s.ballX = ball.GetPosition().x;
    s.ballY = ball.GetPosition().y;
    s.ballSpeedX = ball.speed.x;
    s.ballSpeedY = ball.speed.y;

    s.paddle1X = paddle1.GetRect().x;
    s.paddle2X = paddle2.GetRect().x;

    s.score = score;
    s.lives = lives;

    for (int i = 0; i < bricks.size() && i < 40; i++) {
        s.brickActive[i] = bricks[i].IsActive();
    }

    memcpy(buffer, &s, sizeof(GameSyncState));
}

// ===================== 【老师要求：反序列化 + 缓存前后状态】 =====================
void Game::DeserializeState(unsigned char* buffer) {
    // 旧目标 → 变成上一帧
    prevState = targetState;

    // 新数据 → 变成新目标
    GameSyncState s{};
    memcpy(&s, buffer, sizeof(GameSyncState));
    targetState = s;

    // 重置插值计时器
    interpTimer = 0.0f;
}