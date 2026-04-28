#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "PowerUp.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>
#include <mutex>

std::mutex powerUpMutex;

int main() {
    srand((unsigned int)time(NULL));
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "打砖块 道具完整版");

    std::atomic<bool> gameRunning{true};
    std::vector<Ball> balls;
    balls.emplace_back(Vector2{400, 300}, Vector2{2.5f, 2.5f}, 10);
    Paddle paddle(350, 550, 100, 20);

    std::vector<Brick> bricks;
    float brickWidth = 80;
    float brickHeight = 30;
    for (int row = 0; row < 3; row++) {
        for (int i = 0; i < 8; i++) {
            bricks.emplace_back(60 + i * 90, 100 + row*40, brickWidth, brickHeight);
        }
    }

    std::vector<PowerUp> powerUps;
    std::thread powerUpThread([&](){
        while(gameRunning){
            std::this_thread::sleep_for(std::chrono::seconds(3));
            if(!gameRunning) break;
            float x=(float)(rand()%(700-50+1)+50);
            PowerUpType type=static_cast<PowerUpType>(rand()%3);
            std::lock_guard<std::mutex>lock(powerUpMutex);
            powerUps.emplace_back(x,50.0f,type);
        }
    });
    powerUpThread.detach();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        paddle.Update(dt);

        for (auto& ball : balls) {
            ball.Move();
            ball.BounceEdge(screenWidth, screenHeight);
            ball.UpdateSpeed(dt);
        }

        if (IsKeyDown(KEY_LEFT)) paddle.MoveLeft(5);
        if (IsKeyDown(KEY_RIGHT)) paddle.MoveRight(5);

        // 球碰砖块
        for (auto& ball : balls) {
            for (auto& brick : bricks) {
                if (!brick.IsActive()) continue;
                if (ball.CheckBrickCollision(brick.GetRect())) {
                    brick.SetActive(false);
                    if (rand() % 100 < 30) {
                        PowerUpType type = static_cast<PowerUpType>(rand() % 3);
                        powerUps.emplace_back(brick.GetRect().x, brick.GetRect().y, type);
                    }
                }
            }
        }

        // 道具更新
        std::lock_guard<std::mutex> lock(powerUpMutex);
        for (int i = (int)powerUps.size() - 1; i >= 0; i--) {
            PowerUp& pu = powerUps[i];
            if (!pu.active) continue;
            pu.Update(dt);

            if (CheckCollisionRecs(pu.rect, paddle.GetRect())) {
                pu.active = false;
                switch (pu.type) {
                    case PowerUpType::PADDLE_EXTEND:
                        paddle.Extend(40.0f, 5.0f);
                        break;
                    case PowerUpType::MULTI_BALL:
                    {
                        Ball& mainBall = balls[0];
                        Vector2 newPos = mainBall.GetPos();
                        Vector2 newSpeed = {(float)(rand()%4-2), (float)(rand()%4-2)};
                        if (newSpeed.x==0) newSpeed.x=2.5f;
                        if (newSpeed.y==0) newSpeed.y=-2.5f;
                        balls.emplace_back(newPos, newSpeed, 10);
                        break;
                    }
                    case PowerUpType::SLOW_BALL:
                    {
                        for (auto& ball : balls) ball.SlowDown(0.7f,5.0f);
                        break;
                    }
                }
            }
            if (pu.IsOutOfScreen(screenHeight)) powerUps.erase(powerUps.begin()+i);
        }

        // 球碰挡板
        for (auto& ball : balls) {
            Rectangle brect = { ball.GetPos().x - 10, ball.GetPos().y - 10, 20, 20 };
            if (CheckCollisionRecs(brect, paddle.GetRect())) {
                ball.ReverseY();
            }
        }

        // 球掉到底部
        for (auto& ball : balls) {
            if (ball.GetPos().y > screenHeight + 50) {
                gameRunning = false;
                break;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(0,0,5,screenHeight,GRAY);
        DrawRectangle(screenWidth-5,0,5,screenHeight,GRAY);
        DrawRectangle(0,0,screenWidth,5,GRAY);

        for (auto& brick : bricks) brick.Draw();
        for (auto& ball : balls) ball.Draw();
        paddle.Draw();
        for (auto& pu : powerUps) pu.Draw();
        EndDrawing();
    }

    gameRunning = false;
    CloseWindow();
    return 0;
}