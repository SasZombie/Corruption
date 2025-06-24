#include "raylib.h"

int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Raylib Image Glitch Effect");

    Image img = LoadImage("test.png");
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

    Shader glitchShader = LoadShader(0, "shaders/glitch.fs");
    int redShiftLoc = GetShaderLocation(glitchShader, "redShift");

    if (redShiftLoc == -1)
    {
        return -1;
    }

    float redShift[2] = {0.01f, 0.f};
    SetShaderValue(glitchShader, redShiftLoc, redShift, SHADER_UNIFORM_VEC2);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginTextureMode(target);
        DrawTexture(texture, (screenWidth - texture.width) / 2, (screenHeight - texture.height) / 2, WHITE);
        DrawRectangle(0, 0, 200, 200, RED);
        EndTextureMode();

        BeginDrawing();
        BeginShaderMode(glitchShader);
        DrawTextureRec(target.texture, (Rectangle){0, 0, (float)screenWidth, -(float)screenHeight}, (Vector2){0, 0}, WHITE);
        EndShaderMode();
        EndDrawing();
    }

    UnloadShader(glitchShader);
    UnloadRenderTexture(target);
    UnloadTexture(texture);
    CloseWindow();
}
