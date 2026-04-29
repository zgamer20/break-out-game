#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

// ===================== 【作业要求】多线程异步加载 =====================
#include <thread>
#include <mutex>
#include <chrono>

std::vector<Color> brickRandColors;
bool brickColorInit = false;

std::mutex loadMutex;
bool isLoading = false;
bool textureLoaded = false;

void SimulateLoadTexture() {
    {
        std::lock_guard<std::mutex> lock(loadMutex);
        isLoading = true;
        textureLoaded = false;
    }

    // 模拟3秒加载
    std::this_thread::sleep_for(std::chrono::seconds(3));

    {
        std::lock_guard<std::mutex> lock(loadMutex);
        isLoading = false;
        textureLoaded = true;
    }
}
// ====================================================================

struct ScoreEntry {
    char name[32];
    int score;
    time_t timestamp;
};

class Leaderboard {
private:
    static const int MAX_ENTRIES = 10;
    ScoreEntry entries[MAX_ENTRIES];
    int count;
    const char* filename;
    
public:
    Leaderboard(const char* file) : count(0), filename(file) { Load(); }
    
    void Load() {
        FILE* f = fopen(filename, "r");
        if (f) {
            count = 0;
            while (count < MAX_ENTRIES && fscanf(f, "%31s %d %ld", entries[count].name, &entries[count].score, &entries[count].timestamp) == 3) {
                count++;
            }
            fclose(f);
        }
    }
    
    void Save() {
        FILE* f = fopen(filename, "w");
        if (f) {
            for (int i = 0; i < count; i++) fprintf(f, "%s %d %ld\n", entries[i].name, entries[i].score, entries[i].timestamp);
            fclose(f);
        }
    }
    
    int AddScore(const char* name, int score) {
        if (count >= MAX_ENTRIES && score <= entries[count - 1].score) return 0;
        ScoreEntry newEntry;
        strncpy(newEntry.name, name, 31); newEntry.name[31] = '\0';
        newEntry.score = score; newEntry.timestamp = time(nullptr);
        int pos = 0;
        while (pos < count && entries[pos].score >= score) pos++;
        if (count < MAX_ENTRIES) count++;
        for (int i = count - 1; i > pos; i--) entries[i] = entries[i - 1];
        entries[pos] = newEntry;
        Save();
        return pos + 1;
    }
    
    bool GetEntry(int rank, ScoreEntry& entry) { if (rank > 0 && rank <= count) { entry = entries[rank - 1]; return true; } return false; }
    int GetCount() { return count; }
    bool CanEnter(int score) { return count < MAX_ENTRIES || score > entries[count - 1].score; }
};

int CalculateScore(int baseScore, float gameTime) {
    float multiplier = 5.0f - gameTime * 0.05f;
    if (multiplier < 1.0f) multiplier = 1.0f;
    return (int)(baseScore * multiplier);
}

int main() {
    const int screenWidth = 850, screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Breakout - 作业版：多线程异步加载");
    Leaderboard leaderboard("scores.txt");
    
    Ball ball({400.0f, 530.0f}, {0.0f, 0.0f}, 10.0f);
    Paddle paddle(340.0f, 550.0f, 120.0f, 15.0f);
    
    std::vector<Brick> bricks;
    Color brickColors[] = {RED, ORANGE, YELLOW, GREEN, BLUE};
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 8; col++) {
            bricks.emplace_back(50 + col * 95, 80 + row * 35, 85, 25, brickColors[row]);
        }
    }
    
    int score = 0, lives = 3, winCount = (int)bricks.size(), playerRank = 0;
    bool gameOver = false, paused = false, victory = false, showLeaderboard = false;
    float gameTime = 0.0f;
    
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P) && !gameOver) paused = !paused;
        if (IsKeyPressed(KEY_R)) {
            ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
            score = 0; lives = 3; gameOver = false; victory = false; paused = false; showLeaderboard = false; gameTime = 0.0f;
            bricks.clear();
            for (int row = 0; row < 5; row++) for (int col = 0; col < 8; col++) bricks.emplace_back(50 + col * 95, 80 + row * 35, 85, 25, brickColors[row]);
            winCount = (int)bricks.size();
        }
        if (IsKeyPressed(KEY_L)) {
            // ===================== 【作业】按L触发异步加载 =====================
            std::lock_guard<std::mutex> lock(loadMutex);
            if (!isLoading && !textureLoaded) {
                std::thread loadThread(SimulateLoadTexture);
                loadThread.detach();
            }
        }
        
        if (!gameOver && !paused) {
            if (ball.IsLaunched()) gameTime += GetFrameTime();
            float currentSpeed = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? 28.0f : 18.0f;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) paddle.MoveLeft(currentSpeed);
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) paddle.MoveRight(currentSpeed);
            
            if (!ball.IsLaunched()) {
                ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
                if (IsKeyPressed(KEY_SPACE)) ball.Launch(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().width);
            }
            
            ball.ApplyGravity();
            ball.Move();
            ball.BounceEdge(screenWidth, screenHeight);
            ball.BouncePaddle(paddle.GetRect());

            for (auto& brick : bricks) {
                if (brick.IsActive() && ball.CheckBrickCollision(brick.GetRect())) {
                    brick.SetActive(false); score += CalculateScore(10, gameTime); winCount--; break;
                }
            }
            if (winCount <= 0) { gameOver = true; victory = true; if (leaderboard.CanEnter(score)) playerRank = leaderboard.AddScore("Player", score); }
            if (ball.GetPosition().y > screenHeight + 50) {
                lives--; score -= 50; if (score < 0) score = 0;
                if (lives <= 0) { gameOver = true; victory = false; if (leaderboard.CanEnter(score)) playerRank = leaderboard.AddScore("Player", score); }
                else ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
            }
        }
        
        BeginDrawing();
        ClearBackground(Color{30, 30, 40, 255});
        DrawRectangle(0, 0, 5, screenHeight, GRAY); DrawRectangle(screenWidth - 5, 0, 5, screenHeight, GRAY); DrawRectangle(0, 0, screenWidth, 5, GRAY);
        
        // ===================== 【作业】加载完成后砖块变色 =====================
        for (auto& brick : bricks)
        {
            bool loadedFlag = false;
            {
                std::lock_guard<std::mutex> lock(loadMutex);
                loadedFlag = textureLoaded;
            }

            if (loadedFlag)
            {
                // 只初始化一次随机颜色
                if (!brickColorInit)
                {
                    brickRandColors.clear();
                    for (int i = 0; i < bricks.size(); i++)
                    {
                        unsigned char r = (unsigned char)GetRandomValue(40, 255);
                        unsigned char g = (unsigned char)GetRandomValue(40, 255);
                        unsigned char b = (unsigned char)GetRandomValue(40, 255);
                        brickRandColors.push_back({r, g, b, 255});
                    }
                    brickColorInit = true;
                }

                // 找到当前砖块下标
                int idx = &brick - &bricks[0];
                if (brick.IsActive())
                {
                    DrawRectangleRec(brick.GetRect(), brickRandColors[idx]);
                    DrawRectangleLinesEx(brick.GetRect(), 1, WHITE);
                }
            }
            else
            {
                brick.Draw();
            }
        }

        paddle.Draw();
        ball.Draw();
        
        DrawText("Score:", 20, 8, 24, WHITE);
        DrawText(TextFormat("%d", score), 80, 10, 24, YELLOW);
        
        DrawText("Lives:", 650, 8, 24, WHITE);
        DrawText(TextFormat("%d", lives), 710, 10, 24, lives > 1 ? GREEN : RED);
        
        DrawText("Time:", 20, 35, 20, Fade(WHITE, 0.8f));
        DrawText(TextFormat("%.1fs", gameTime), 75, 37, 20, Fade(WHITE, 0.8f));
        
        float currentMultiplier = 5.0f - gameTime * 0.05f;
        if (currentMultiplier < 1.0f) currentMultiplier = 1.0f;
        
        DrawText(TextFormat("x%.1f", currentMultiplier), 140, 37, 20, currentMultiplier > 2.0f ? GREEN : Fade(WHITE, 0.5f));
        
        if (!ball.IsLaunched()) {
            DrawText("PRESS SPACE", screenWidth/2 - MeasureText("PRESS SPACE",20)/2, 55, 20, YELLOW);
        }
        
        DrawText("P-Pause  R-Restart  L-Load", 270, 12, 18, Fade(WHITE, 0.6f));
        
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            DrawText(">>> BOOST <<<", 350, 575, 18, YELLOW);
        }
        
        if (paused && !gameOver) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
            DrawText("PAUSED", screenWidth/2 - MeasureText("PAUSED",48)/2, screenHeight/2 - 40, 48, YELLOW);
            DrawText("Press P to continue", screenWidth/2 - MeasureText("Press P to continue",24)/2, screenHeight/2 + 30, 24, WHITE);
        }
        
        if (gameOver) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.85f));
            
            if (victory) {
                DrawText("YOU WIN!", screenWidth/2 - MeasureText("YOU WIN!",48)/2, screenHeight/2 - 80, 48, GREEN);
                DrawText(TextFormat("FINAL SCORE: %d", score), screenWidth/2 - 100, screenHeight/2 - 30, 28, YELLOW);
            } else {
                DrawText("GAME OVER", screenWidth/2 - MeasureText("GAME OVER",48)/2, screenHeight/2 - 80, 48, RED);
                DrawText(TextFormat("SCORE: %d", score), screenWidth/2 - 60, screenHeight/2 - 30, 28, YELLOW);
            }
            
            if (playerRank > 0) {
                char rankText[64];
                sprintf(rankText, "You ranked #%d in leaderboard!", playerRank);
                DrawText(rankText, screenWidth/2 - MeasureText(rankText,24)/2, screenHeight/2 + 40, 24, GOLD);
            }
            
            DrawText("Press R to restart", screenWidth/2 - MeasureText("Press R to restart",24)/2, screenHeight/2 + 90, 24, WHITE);
        }
        
        // ===================== 【作业】显示加载状态 =====================
        {
            std::lock_guard<std::mutex> lock(loadMutex);
            if (isLoading) {
                DrawText("LOADING...", screenWidth/2 - 80, screenHeight/2 - 20, 40, YELLOW);
            }
            if (textureLoaded) {
                DrawText("LOADED!", screenWidth/2 - 60, 20, 30, GREEN);
            }
        }
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}