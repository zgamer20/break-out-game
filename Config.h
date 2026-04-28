#pragma once
#include "raylib.h"
// 这是一个“配置中心”，所有的数字都集中在这里管理！
namespace Config {
    // 窗口配置
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const int FPS = 60;

    // 球的配置
    const float BALL_RADIUS = 10.0f;
    const float BALL_SPEED_X = 4.0f;
    const float BALL_SPEED_Y = -4.0f;

    // 游戏配置
    const int START_LIVES = 3;
    const int BRICK_SCORE = 10;
    const Color ROW_COLORS[] = { RED, ORANGE, YELLOW, GREEN, BLUE };
}