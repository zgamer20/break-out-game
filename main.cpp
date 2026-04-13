#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "PowerUp.h"   // 道具系统
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>

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
            for (int i = 0; i < count; i++)
                fprintf(f, "%s %d %ld\n", entries[i].name, entries[i].score, entries[i].timestamp);
            fclose(f);
        }
    }

    int AddScore(const char* name, int score) {
        if (count >= MAX_ENTRIES && score <= entries[count - 1].score) return 0;
        ScoreEntry ne;
        strncpy(ne.name, name, 31); ne.name[31] = 0;
        ne.score = score; ne.timestamp = time(NULL);
        int pos = 0;
        while (pos < count && entries[pos].score >= score) pos++;
        if (count < MAX_ENTRIES) count++;
        for (int i = count - 1; i > pos; i--) entries[i] = entries[i - 1];
        entries[pos] = ne;
        Save();
        return pos + 1;
    }

    bool GetEntry(int rank, ScoreEntry& e) {
        if (rank > 0 && rank <= count) { e = entries[rank - 1]; return true; }
        return false;
    }

    int GetCount() { return count; }
    bool CanEnter(int s) { return count < MAX_ENTRIES || s > entries[count - 1].score; }
};

static Font chineseFont;
static bool fontLoaded = false;

void InitChineseFont() {
    const char* text = "分数生命暂停继续重新开始游戏结束胜利排行榜第名按P暂停按R重新开始时间倍率落地惩罚恭喜进入空格发射等待加长中减速中";
    int cnt = 0;
    int* codes = LoadCodepoints(text, &cnt);
    const char* paths[] = {
        "Noto-Sans-SC-Bold-2.ttf",
        "../Noto-Sans-SC-Bold-2.ttf",
        "/usr/share/fonts/opentype/noto/NotoSansSC-Regular.otf",
        "C:/Windows/Fonts/msyh.ttc",
        "/System/Library/Fonts/PingFang.ttc"
    };
    chineseFont = GetFontDefault();
    for (int i = 0; i < 5; i++) {
        if (FileExists(paths[i])) {
            chineseFont = LoadFontEx(paths[i], 48, codes, cnt);
            if (chineseFont.texture.id != 0) { fontLoaded = true; break; }
        }
    }
    UnloadCodepoints(codes);
}

void DrawChineseText(const char* t, int x, int y, int sz, Color c) {
    DrawTextEx(chineseFont, t, {(float)x,(float)y}, sz,1,c);
}

void DrawChineseTextCentered(const char* t, int y, int sz, Color c) {
    Vector2 szv = MeasureTextEx(chineseFont,t,sz,1);
    DrawTextEx(chineseFont,t,{(GetScreenWidth()-szv.x)/2,(float)y},sz,1,c);
}

int CalculateScore(int base, float time) {
    float m = 5.0f - time * 0.05f;
    if (m < 1) m = 1;
    return (int)(base * m);
}

int main() {
    const int w = 800, h = 600;
    InitWindow(w, h, "Breakout + PowerUps");
    InitChineseFont();
    srand(time(NULL));
    Leaderboard lb("scores.txt");
    Game game;
    int rank = 0;
    bool pause = false, showLb = false;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P) && !game.gameOver) pause = !pause;
        if (IsKeyPressed(KEY_R)) { game.Reset(); rank = 0; pause = false; showLb = false; }
        if (IsKeyPressed(KEY_L)) showLb = !showLb;

        if (!game.gameOver && !pause) {
            game.Update(GetFrameTime());
            if (game.gameOver && lb.CanEnter(game.score)) rank = lb.AddScore("Player", game.score);
        }

        BeginDrawing();
        ClearBackground({30,30,40,255});

        if (showLb) {
            DrawRectangle(0,0,w,h,Fade(BLACK,0.9f));
            DrawChineseTextCentered("排行榜",40,36,GOLD);
            DrawText("RANK",150,90,20,Fade(WHITE,0.6f));
            DrawText("NAME",250,90,20,Fade(WHITE,0.6f));
            DrawText("SCORE",450,90,20,Fade(WHITE,0.6f));
            DrawText("DATE",550,90,20,Fade(WHITE,0.6f));
            DrawLine(150,115,700,115,Fade(WHITE,0.3f));
            for(int i=0;i<lb.GetCount()&&i<10;i++){
                ScoreEntry e; lb.GetEntry(i+1,e);
                int y=130+i*35;
                Color rc = (i==0)?GOLD:(i==1)?LIGHTGRAY:(i==2)?ORANGE:WHITE;
                DrawText(TextFormat("#%d",i+1),150,y,22,rc);
                DrawText(e.name,250,y,22,rc);
                DrawText(TextFormat("%d",e.score),450,y,22,rc);
                char buf[32]; strftime(buf,32,"%m/%d",localtime(&e.timestamp));
                DrawText(buf,550,y,20,Fade(rc,0.7f));
            }
            DrawChineseTextCentered("按 L 关闭",h-50,20,Fade(WHITE,0.5f));
        } else if (pause) {
            DrawRectangle(0,0,w,h,Fade(BLACK,0.7f));
            DrawChineseTextCentered("暂停",h/2-40,48,YELLOW);
            DrawChineseTextCentered("按 P 继续",h/2+30,24,WHITE);
            game.Draw();
        } else if (game.gameOver) {
            DrawRectangle(0,0,w,h,Fade(BLACK,0.85f));
            if (game.victory) {
                DrawChineseTextCentered("胜利",h/2-80,48,GREEN);
                DrawText(TextFormat("分数:%d",game.score),w/2-80,h/2-30,28,YELLOW);
            } else {
                DrawChineseTextCentered("游戏结束",h/2-80,48,RED);
                DrawText(TextFormat("分数:%d",game.score),w/2-60,h/2-30,28,YELLOW);
            }
            if(rank>0){
                char buf[64]; sprintf(buf,"恭喜进入排行榜第%d名!",rank);
                DrawChineseTextCentered(buf,h/2+40,24,GOLD);
            }
            DrawChineseTextCentered("按 R 重新开始",h/2+90,24,WHITE);
            game.Draw();
        } else {
            game.Draw();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}