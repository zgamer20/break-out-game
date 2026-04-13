#include "PowerUp.h"
#include "Brick.h"
#include <cmath>
#include <cstring>
#include <string>

using namespace std;

extern int CalculateScore(int base, float time);
extern void DrawChineseText(const char* text, int x, int y, int fontSize, Color color);
extern void DrawChineseTextCentered(const char* t, int y, int sz, Color c);

PowerUp::PowerUp(float x, float y, PowerUpType t)
    : position({x, y}), type(t), active(true), duration(5.0f), speed(150.0f)
{
    if (type == PowerUpType::MULTI_BALL) duration = 0.0f;
}

void PowerUp::Update(float dt) {
    if (!active) return;
    position.y += speed * dt;
    if (position.y > GetScreenHeight()) active = false;
}

void PowerUp::Draw() {
    if (!active) return;
    Color color = WHITE;
    switch(type) {
        case PowerUpType::PADDLE_EXTEND: color = GREEN; break;
        case PowerUpType::MULTI_BALL: color = YELLOW; break;
        case PowerUpType::SLOW_BALL: color = BLUE; break;
        default: color = WHITE;
    }
    DrawCircleV(position, 25, Fade(color, 0.3f));
    DrawRectangle(position.x - 15, position.y - 15, 30, 30, color);
    DrawRectangleLines(position.x - 15, position.y - 15, 30, 30, WHITE);
}

bool PowerUp::CheckPaddleCollision(const Rectangle& paddleRect) {
    if (!active) return false;
    Rectangle r = { position.x - 15, position.y - 15, 30, 30 };
    return CheckCollisionRecs(r, paddleRect);
}

void PowerUp::Apply(Game& game) {
    if (!active) return;
    active = false;

    switch (type) {
        case PowerUpType::PADDLE_EXTEND:
            game.paddle.Extend(40, 5);
            break;
        case PowerUpType::MULTI_BALL:
            if (!game.balls.empty()) {
                Ball b = game.balls[0];
                b.SetPosition({ b.GetPosition().x + 10, b.GetPosition().y });
                float angle = GetRandomValue(-30, 30) * DEG2RAD;
                b.SetDirection(Vector2{ (float)cosf(angle), (float)sinf(angle) });
                game.balls.push_back(b);
            }
            break;
        case PowerUpType::SLOW_BALL:
            game.slowBallRemainingTime = 5.0f;
            for (auto& ball : game.balls)
                ball.SetSpeed(game.originalBallSpeed * 0.7f);
            break;
    }
}

Game::Game()
    : paddle(340, 550, 120, 15), score(0), lives(3), gameOver(false), victory(false),
      gameTime(0), slowBallRemainingTime(0), originalBallSpeed(300)
{
    balls.emplace_back(Vector2{ 400, 530 }, Vector2{ 0,0 }, 10);
    balls[0].SetSpeed(originalBallSpeed);

    Color cs[] = { RED, ORANGE, YELLOW, GREEN, BLUE };
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 8; c++)
            bricks.emplace_back(50 + c * 95, 80 + r * 35, 85, 25, cs[r]);
}

void Game::Reset() {
    score = 0;
    lives = 3;
    gameOver = false;
    victory = false;
    gameTime = 0;
    slowBallRemainingTime = 0;

    balls.clear();
    balls.emplace_back(Vector2{ 400,530 }, Vector2{ 0,0 }, 10);
    balls[0].SetSpeed(originalBallSpeed);
    balls[0].ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);

    paddle.Reset();
    bricks.clear();
    Color cs[] = { RED, ORANGE, YELLOW, GREEN, BLUE };
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 8; c++)
            bricks.emplace_back(50 + c * 95, 80 + r * 35, 85, 25, cs[r]);
    powerUps.clear();
    particles.clear();  // 重置游戏时清空所有粒子
}

void Game::Update(float dt) {
    if (gameOver) return;

    if (!balls.empty() && balls[0].IsLaunched())
        gameTime += dt;

    float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 28 : 18;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) paddle.MoveLeft(speed * 60 * dt);
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) paddle.MoveRight(speed * 60 * dt);
    paddle.Update(dt);

    if (slowBallRemainingTime > 0) {
        slowBallRemainingTime -= dt;
        if (slowBallRemainingTime <= 0)
            for (auto& b : balls) b.SetSpeed(originalBallSpeed);
    }

    for (auto it = balls.begin(); it != balls.end();) {
        Ball& b = *it;
        if (!b.IsLaunched()) {
            b.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
            if (IsKeyPressed(KEY_SPACE))
                b.Launch(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().width);
            ++it;
            continue;
        }

        b.ApplyGravity();
        b.Move();
        b.BounceEdge(GetScreenWidth(), GetScreenHeight());
        b.BouncePaddle(paddle.GetRect());

        bool hit = false;
        for (auto& brick : bricks) {
            if (brick.IsActive() && b.CheckBrickCollision(brick.GetRect())) {
                brick.SetActive(false);
                score += CalculateScore(10, gameTime);
                hit = true;
                Rectangle brickRect = brick.GetRect();
                Color brickColor = brick.GetColor();
                for (int i = 0; i < 10; i++) {
                    Particle p;
                    p.pos = { brickRect.x + rand()%85, brickRect.y + rand()%25 };
                    p.vel = { (rand()%100 - 50) / 10.0f, (rand()%100 - 50) / 10.0f };
                    p.color = brickColor;
                    p.life = 0.5f;
                    particles.push_back(p);
                }
                if (GetRandomValue(0, 99) < 30) {
                    PowerUpType t = static_cast<PowerUpType>(GetRandomValue(0, 2));
                    powerUps.emplace_back(brick.GetRect().x + brick.GetRect().width/2, brick.GetRect().y, t);
                }
                break;
            }
        }

        if (b.GetPosition().y > GetScreenHeight() + 50) {
            it = balls.erase(it);
            if (balls.empty()) {
                lives--;
                score -= 50;
                if (score < 0) score = 0;
                if (lives <= 0) gameOver = true;
                else {
                    balls.emplace_back(Vector2{ 400,530 }, Vector2{ 0,0 }, 10);
                    balls.back().SetSpeed(slowBallRemainingTime > 0 ? originalBallSpeed*0.7f : originalBallSpeed);
                    balls.back().ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width/2, paddle.GetRect().y);
                }
            }
        } else {
            ++it;
        }
    }

    int alive = 0;
    for (auto& br : bricks) if (br.IsActive()) alive++;
    if (alive == 0) { gameOver = true; victory = true; }

    for (auto it = powerUps.begin(); it != powerUps.end();) {
        auto& pu = *it;
        pu.Update(dt);
        if (pu.CheckPaddleCollision(paddle.GetRect())) {
            pu.Apply(*this);
            it = powerUps.erase(it);
        } else if (!pu.active) {
            it = powerUps.erase(it);
        } else {
            ++it;
        }
    }

    // 粒子更新：每帧减少生命周期，移除死亡粒子
    for (auto it = particles.begin(); it != particles.end();) {
        Particle& p = *it;
        p.life -= dt;          // 生命周期倒计时
        if (p.life <= 0) {
            it = particles.erase(it);  // 生命周期到0，删除粒子
        } else {
            p.pos.x += p.vel.x * dt;   // 按速度移动
            p.pos.y += p.vel.y * dt;
            p.vel.y += 200.0f * dt;    // 给粒子加重力，模拟下落
            ++it;
        }
    }
}

void Game::Draw() {
    DrawRectangle(0, 0, 5, GetScreenHeight(), GRAY);
    DrawRectangle(GetScreenWidth() - 5, 0, 5, GetScreenHeight(), GRAY);
    DrawRectangle(0, 0, GetScreenWidth(), 5, GRAY);

    for (auto& br : bricks) br.Draw();
    paddle.Draw();
    for (auto& b : balls) b.Draw();
    for (auto& pu : powerUps) pu.Draw();
    // 绘制所有粒子
    for (auto& p : particles) {
        float alpha = p.life / 0.5f;  // 生命周期越短，越透明
        Color fadeColor = Color{
            p.color.r,
            p.color.g,
            p.color.b,
            (unsigned char)(255 * alpha)
        };
        DrawCircleV(p.pos, 5.0f, fadeColor);  // 画圆形粒子（适配放大2倍，可改8-10）
    }

    DrawChineseText("分数:", 20, 8, 24, WHITE);
    DrawText(TextFormat("%d", score), 80, 10, 24, YELLOW);
    DrawChineseText("生命:", 650, 8, 24, WHITE);
    DrawText(TextFormat("%d", lives), 710, 10, 24, lives > 1 ? GREEN : RED);

    if (slowBallRemainingTime > 0) {
        DrawChineseText("减速中", 200, 8, 24, BLUE);
        DrawText(TextFormat("%.1f", slowBallRemainingTime), 260, 10, 24, BLUE);
    }
    if (paddle.GetEffectRemainingTime() > 0) {
        DrawChineseText("加长中", 320, 8, 24, GREEN);
        DrawText(TextFormat("%.1f", paddle.GetEffectRemainingTime()), 380, 10, 24, GREEN);
    }

    if (!balls.empty() && !balls[0].IsLaunched()) {
        DrawChineseTextCentered("按空格发射", 55, 20, YELLOW);
    }
}