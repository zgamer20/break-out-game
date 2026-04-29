#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

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
    InitWindow(screenWidth, screenHeight, "Breakout - Enhanced Edition");
    Leaderboard leaderboard("scores.txt");
    
    // ====================== 小球速度降低（直接在这里改）======================
    Ball ball({400.0f, 530.0f}, {0.0f, 0.0f}, 10.0f);
    
    // 游戏主逻辑
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
        if (IsKeyPressed(KEY_L)) showLeaderboard = !showLeaderboard;
        
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
        for (auto& brick : bricks) brick.Draw();
        paddle.Draw(); ball.Draw();
        
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
        
        DrawText("P-Pause  R-Restart  L-Leaderboard", 270, 12, 18, Fade(WHITE, 0.6f));
        
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
        
        if (showLeaderboard) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.9f));
            DrawText("LEADERBOARD", screenWidth/2 - MeasureText("LEADERBOARD",36)/2, 40, 36, GOLD);
            
            DrawText("RANK", 150, 90, 20, Fade(WHITE, 0.6f));
            DrawText("NAME", 250, 90, 20, Fade(WHITE, 0.6f));
            DrawText("SCORE", 450, 90, 20, Fade(WHITE, 0.6f));
            DrawText("DATE", 550, 90, 20, Fade(WHITE, 0.6f));
            
            DrawLine(150, 115, 700, 115, Fade(WHITE, 0.3f));
            
            for (int i = 0; i < leaderboard.GetCount() && i < 10; i++) {
                ScoreEntry entry;
                if (leaderboard.GetEntry(i + 1, entry)) {
                    int y = 130 + i * 35;
                    Color rowColor = (i == 0) ? GOLD : (i == 1) ? Color{192,192,192,255} : (i == 2) ? Color{205,127,50,255} : WHITE;
                    
                    DrawText(TextFormat("#%d", i + 1), 150, y, 22, rowColor);
                    DrawText(entry.name, 250, y, 22, rowColor);
                    DrawText(TextFormat("%d", entry.score), 450, y, 22, rowColor);
                    
                    char dateStr[32];
                    strftime(dateStr, sizeof(dateStr), "%m/%d", localtime(&entry.timestamp));
                    DrawText(dateStr, 550, y, 20, Fade(rowColor, 0.7f));
                }
            }
            
            if (leaderboard.GetCount() == 0) {
                DrawText("No scores yet", screenWidth/2 - MeasureText("No scores yet",24)/2, screenHeight/2, 24, Fade(WHITE, 0.5f));
            }
            
            DrawText("Press L to close", screenWidth/2 - MeasureText("Press L to close",20)/2, screenHeight - 50, 20, Fade(WHITE, 0.5f));
        }
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}