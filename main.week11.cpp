#include "raylib.h"
#include "Ball.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <ctime>
#include <cstdio>
#include <cstring>

using json = nlohmann::json;
using namespace std;

const int SCREEN_W = 850;
const int SCREEN_H = 600;
const int MAX_LEVEL = 3;       // 必须3个关卡
const int DEFAULT_LIVES = 3;

// -------------------------- 合并在头文件的Paddle/Brick实现（适配你们的结构） --------------------------
class Brick {
private:
    Rectangle rect;
    bool active;
    Color color;
public:
    Brick(float x, float y, float w, float h, Color c) 
        : rect{x, y, w, h}, active(true), color(c) {}
    void Draw() {
        if (active) {
            DrawRectangleRec(rect, color);
            DrawRectangleLinesEx(rect, 1, BLACK);
        }
    }
    bool IsActive() { return active; }
    void SetActive(bool v) { active = v; }
    Rectangle GetRect() { return rect; }
};

class Paddle {
private:
    Rectangle rect;
public:
    Paddle(float x, float y, float w, float h) : rect{x, y, w, h} {}
    void Draw() { DrawRectangleRec(rect, WHITE); }
    void MoveLeft(float speed) {
        rect.x -= speed;
        if (rect.x < 0) rect.x = 0;
    }
    void MoveRight(float speed) {
        rect.x += speed;
        if (rect.x + rect.width > SCREEN_W) rect.x = SCREEN_W - rect.width;
    }
    Rectangle GetRect() { return rect; }
};

// -------------------------- 排行榜（保留原功能） --------------------------
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
            while (count < MAX_ENTRIES && fscanf(f, "%31s %d %ld", entries[count].name, &entries[count].score, &entries[count].timestamp) == 3)
                count++;
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
    bool GetEntry(int rank, ScoreEntry& entry) { 
        if (rank > 0 && rank <= count) { entry = entries[rank - 1]; return true; } 
        return false; 
    }
    int GetCount() { return count; }
    bool CanEnter(int score) { return count < MAX_ENTRIES || score > entries[count - 1].score; }
};

// -------------------------- 工具函数 --------------------------
int CalculateScore(int baseScore, float gameTime) {
    float multiplier = 5.0f - gameTime * 0.05f;
    if (multiplier < 1.0f) multiplier = 1.0f;
    return (int)(baseScore * multiplier);
}

Color GetColorFromStr(const string& name) {
    if (name == "red") return RED;
    if (name == "orange") return ORANGE;
    if (name == "yellow") return YELLOW;
    if (name == "green") return GREEN;
    if (name == "blue") return BLUE;
    return GRAY;
}

json LoadJSON(const string& path) {
    try {
        ifstream f(path);
        if (!f) {
            TraceLog(LOG_WARNING, "File not found: %s, using default layout", path.c_str());
            return json();
        }
        json j;
        f >> j;
        return j;
    } catch (const exception& e) {
        TraceLog(LOG_ERROR, "JSON parse error in %s: %s", path.c_str(), e.what());
        return json();
    }
}

// -------------------------- 全局游戏状态 --------------------------
vector<Brick> bricks;
int currentLevel = 1;
int score = 0;
int lives = DEFAULT_LIVES;
bool gameOver = false;
bool victory = false;
bool paused = false;
bool showLeaderboard = false;
bool showContinuePrompt = false;
int playerRank = 0;
float gameTime = 0.0f;

// -------------------------- 核心关卡/存档函数 --------------------------
void LoadLevel(int level) {
    bricks.clear();
    string file = "level" + to_string(level) + ".json";
    json j = LoadJSON(file);

    if (j.empty()) {
        // 缺失JSON时使用默认布局（满足错误处理要求）
        Color brickColors[] = {RED, ORANGE, YELLOW, GREEN, BLUE};
        for (int row = 0; row < 5; row++)
            for (int col = 0; col < 8; col++)
                bricks.emplace_back(50 + col * 95, 80 + row * 35, 85, 25, brickColors[row]);
        return;
    }

    auto& b = j["bricks"];
    int rows = b["rows"];
    int cols = b["cols"];
    int w = b["width"];
    int h = b["height"];
    auto& layout = b["layout"];
    auto& cmap = b["color_map"];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            int t = layout[i][j];
            if (t != 0) {
                string ckey = to_string(t);
                Color col = GetColorFromStr(cmap[ckey]);
                bricks.emplace_back(50 + j * w, 80 + i * h, w, h, col);
            }
        }
}

void SaveGame() {
    json j;
    j["version"] = 1;
    j["current_level"] = currentLevel;
    j["score"] = score;
    j["lives"] = lives;

    ofstream f("save.json");
    if (f.is_open()) {
        f << j.dump(4);
        TraceLog(LOG_INFO, "Game saved successfully");
    } else {
        TraceLog(LOG_ERROR, "Failed to save game");
    }
}

bool LoadGame() {
    json j = LoadJSON("save.json");
    if (j.empty()) return false;

    if (j.contains("version") && j["version"] == 1) {
        currentLevel = j["current_level"];
        score = j["score"];
        lives = j["lives"];
        return true;
    }
    return false;
}

bool AllBricksGone() {
    for (auto& br : bricks) if (br.IsActive()) return false;
    return true;
}

// -------------------------- 主函数 --------------------------
int main() {
    InitWindow(SCREEN_W, SCREEN_H, "Breakout - Week11 Enhanced");
    Leaderboard leaderboard("scores.txt");

    // 读档询问（满足“启动时询问是否继续”要求）
    bool hasSave = LoadGame();
    if (hasSave) {
        showContinuePrompt = true;
    } else {
        currentLevel = 1;
        score = 0;
        lives = DEFAULT_LIVES;
        LoadLevel(currentLevel);
    }

    Ball ball({400.0f, 530.0f}, {0.0f, 0.0f}, 10.0f);
    Paddle paddle(340.0f, 550.0f, 120.0f, 15.0f);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // 存档询问界面
        if (showContinuePrompt) {
            BeginDrawing();
            ClearBackground({30, 30, 40, 255});
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.7f));
            DrawText("Continue previous game?", SCREEN_W/2 - MeasureText("Continue previous game?", 32)/2, 200, 32, WHITE);
            DrawText("Y - Continue   N - New Game", SCREEN_W/2 - MeasureText("Y - Continue   N - New Game", 24)/2, 260, 24, YELLOW);
            EndDrawing();

            if (IsKeyPressed(KEY_Y)) {
                showContinuePrompt = false;
                LoadLevel(currentLevel);
            }
            if (IsKeyPressed(KEY_N)) {
                showContinuePrompt = false;
                currentLevel = 1;
                score = 0;
                lives = DEFAULT_LIVES;
                LoadLevel(currentLevel);
                remove("save.json");
            }
            continue;
        }

        if(IsKeyPressed(KEY_ONE))
        {
            currentLevel = 1;
            LoadLevel(currentLevel);
            ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
            gameTime = 0.0f;
        }
        if(IsKeyPressed(KEY_TWO))
        {
            currentLevel = 2;
            LoadLevel(currentLevel);
            ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
            gameTime = 0.0f;
        }
        if(IsKeyPressed(KEY_THREE))
        {
            currentLevel = 3;
            LoadLevel(currentLevel);
            ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
            gameTime = 0.0f;
        }

        // 按键控制
        if (IsKeyPressed(KEY_P) && !gameOver) paused = !paused;
        if (IsKeyPressed(KEY_R)) {
            ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
            score = 0; lives = DEFAULT_LIVES; gameOver = false; victory = false; paused = false; showLeaderboard = false; gameTime = 0.0f;
            currentLevel = 1;
            LoadLevel(currentLevel);
            remove("save.json");
        }
        if (IsKeyPressed(KEY_L)) showLeaderboard = !showLeaderboard;
        if (IsKeyPressed(KEY_S)) SaveGame();

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
            ball.BounceEdge(SCREEN_W, SCREEN_H);
            ball.BouncePaddle(paddle.GetRect());

            for (auto& brick : bricks) {
                if (brick.IsActive() && ball.CheckBrickCollision(brick.GetRect())) {
                    brick.SetActive(false); 
                    score += CalculateScore(10, gameTime); 
                    SaveGame(); // 关键操作自动保存
                    break;
                }
            }

            // 关卡通关自动加载下一关（满足3个关卡要求）
            if (AllBricksGone()) {
                if (currentLevel < MAX_LEVEL) {
                    currentLevel++;
                    LoadLevel(currentLevel);
                    ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
                    SaveGame();
                    gameTime = 0.0f;
                } else {
                    gameOver = true;
                    victory = true;
                    if (leaderboard.CanEnter(score)) playerRank = leaderboard.AddScore("Player", score);
                }
            }

            // 生命值处理（防止负数）
            if (ball.GetPosition().y > SCREEN_H + 50) {
                lives--; 
                score -= 50; 
                if (score < 0) score = 0;
                SaveGame();

                if (lives <= 0) {
                    gameOver = true; 
                    victory = false; 
                    if (leaderboard.CanEnter(score)) playerRank = leaderboard.AddScore("Player", score);
                } else {
                    ball.ResetToPaddle(paddle.GetRect().x + paddle.GetRect().width / 2, paddle.GetRect().y);
                }
            }
        }
        
        // 绘制界面
        BeginDrawing();
        ClearBackground(Color{30, 30, 40, 255});
        DrawRectangle(0, 0, 5, SCREEN_H, GRAY); 
        DrawRectangle(SCREEN_W - 5, 0, 5, SCREEN_H, GRAY); 
        DrawRectangle(0, 0, SCREEN_W, 5, GRAY);
        
        for (auto& brick : bricks) brick.Draw();
        paddle.Draw(); 
        ball.Draw();
        
        DrawText("Score:", 20, 8, 24, WHITE);
        DrawText(TextFormat("%d", score), 80, 10, 24, YELLOW);

        DrawText("Lives:", 650, 8, 24, WHITE);
        DrawText(TextFormat("%d", lives), 710, 10, 24, lives > 1 ? GREEN : RED);

        // 下移后的Level文字，不会被挡住
        DrawText("Level:", 380, 30, 24, WHITE);
        DrawText(TextFormat("%d", currentLevel), 440, 32, 24, BLUE);

        // 下移后的Time文字，避免和Level重叠
        DrawText("Time:", 20, 55, 20, Fade(WHITE, 0.8f));
        DrawText(TextFormat("%.1fs", gameTime), 75, 57, 20, Fade(WHITE, 0.8f));

        float currentMultiplier = 5.0f - gameTime * 0.05f;
        if (currentMultiplier < 1.0f) currentMultiplier = 1.0f;
        DrawText(TextFormat("x%.1f", currentMultiplier), 140, 37, 20, currentMultiplier > 2.0f ? GREEN : Fade(WHITE, 0.5f));
        
        if (!ball.IsLaunched()) {
            DrawText("PRESS SPACE", SCREEN_W/2 - MeasureText("PRESS SPACE",20)/2, 55, 20, YELLOW);
        }
        
        DrawText("P-Pause  R-Restart  L-Leaderboard  S-Save", 230, 12, 18, Fade(WHITE, 0.6f));
        
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            DrawText(">>> BOOST <<<", 350, 575, 18, YELLOW);
        }
        
        if (paused && !gameOver) {
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.7f));
            DrawText("PAUSED", SCREEN_W/2 - MeasureText("PAUSED",48)/2, SCREEN_H/2 - 40, 48, YELLOW);
            DrawText("Press P to continue", SCREEN_W/2 - MeasureText("Press P to continue",24)/2, SCREEN_H/2 + 30, 24, WHITE);
        }
        
        if (gameOver) {
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.85f));
            
            if (victory) {
                DrawText("YOU WIN!", SCREEN_W/2 - MeasureText("YOU WIN!",48)/2, SCREEN_H/2 - 80, 48, GREEN);
                DrawText(TextFormat("FINAL SCORE: %d", score), SCREEN_W/2 - 100, SCREEN_H/2 - 30, 28, YELLOW);
            } else {
                DrawText("GAME OVER", SCREEN_W/2 - MeasureText("GAME OVER",48)/2, SCREEN_H/2 - 80, 48, RED);
                DrawText(TextFormat("SCORE: %d", score), SCREEN_W/2 - 60, SCREEN_H/2 - 30, 28, YELLOW);
            }
            
            if (playerRank > 0) {
                char rankText[64];
                sprintf(rankText, "You ranked #%d in leaderboard!", playerRank);
                DrawText(rankText, SCREEN_W/2 - MeasureText(rankText,24)/2, SCREEN_H/2 + 40, 24, GOLD);
            }
            
            DrawText("Press R to restart", SCREEN_W/2 - MeasureText("Press R to restart",24)/2, SCREEN_H/2 + 90, 24, WHITE);
        }
        
        if (showLeaderboard) {
            DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.9f));
            DrawText("LEADERBOARD", SCREEN_W/2 - MeasureText("LEADERBOARD",36)/2, 40, 36, GOLD);
            
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
                DrawText("No scores yet", SCREEN_W/2 - MeasureText("No scores yet",24)/2, SCREEN_H/2, 24, Fade(WHITE, 0.5f));
            }
            
            DrawText("Press L to close", SCREEN_W/2 - MeasureText("Press L to close",20)/2, SCREEN_H - 50, 20, Fade(WHITE, 0.5f));
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}