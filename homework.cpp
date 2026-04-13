#include "raylib.h"
#include <cmath>

int main() {
    // 鍒濆鍖栫獥鍙?
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "鍏浆鐨?D鐞冧綋");

    // 瀹氫箟鎽勫儚鏈?
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    // 涓绘父鎴忓惊鐜?
    while (!WindowShouldClose()) {
        // 璁＄畻鐞冧綋浣嶇疆锛氱粫Y杞存棆杞紝鍗婂緞5锛岄€熷害鍙皟
        float angle = GetTime() * 2.0f;          // 姣忕鏃嬭浆2寮у害锛堢害114搴︼級
        float radius = 5.0f;
        Vector3 spherePos = {
            radius * sinf(angle),
            0.0f,
            radius * cosf(angle)
        };

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
            // 缁樺埗鐞冧綋锛堝疄蹇?绾挎锛?
            DrawSphere(spherePos, 1.0f, RED);
            DrawSphereWires(spherePos, 1.0f, 16, 16, BLACK);
            // 缁樺埗鍙傝€冪綉鏍?
            DrawGrid(10, 1.0f);
        EndMode3D();

        DrawText("鍏浆鐨?D鐞冧綋 - 浣跨敤Raylib", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
