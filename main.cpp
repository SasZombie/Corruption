#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#pragma GCC diagnostic pop

#include <iostream>

#include "colorManipulation.hpp"

struct Circle
{
    float cx;
    float cy;
    float radius;

    Color c;
};

int main()
{
    //Vars
    Image img = LoadImage("test.png");

    constexpr size_t widthUtils = 200;
    const size_t windowWidth = img.width, windowHeight = img.height;
    const size_t px = 15, py = 10;
    const size_t elemPadd = 20;
    float redShift = 0.f;

    InitWindow(windowWidth + widthUtils, img.height, "Image Viewer");
    SetTargetFPS(60);

    //Shaders
    Texture2D texture = LoadTextureFromImage(img);
    Shader shader = LoadShader(0, "shaders/interestingPattern.fs");

    int redShiftLoc = GetShaderLocation(shader, "redShift");
    if (redShiftLoc == -1)
    {
        std::cerr << "Shader uniform redShift not found\n";
    }

    //Drawables
    Rectangle slider{px, py, 5, 255};
    Circle c{px + slider.width / 2, py + slider.height, 10, RED};

    bool isSelected = false;
   

    while (!WindowShouldClose())
    {

        // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        // {
        //     redShift += 0.1f;

        //     if (redShift > 1.f)
        //     {
        //         redShift = 0.f;
        //     }

        //     SetShaderValue(shader, redShiftLoc, &redShift, SHADER_UNIFORM_FLOAT);
        // }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            const Vector2 mpos = GetMousePosition();
            if (CheckCollisionPointCircle(mpos, {c.cx, c.cy}, c.radius) || CheckCollisionPointRec(mpos, slider))
            {
                isSelected = true;
            }
            else
            {
                isSelected = false;
            }
        }

        if (isSelected)
        {

            const auto &[mx, my] = GetMousePosition();
            // std::cout << mx << '|' << my << '\n';
            if (my > py + 255)
            {
                c.cy = py + 255;
            }
            else if (my < py)
            {
                c.cy = py;
            }
            else
            {
                c.cy = my;
            }

            redShift = (c.cy - py) / 255;
            SetShaderValue(shader, redShiftLoc, &redShift, SHADER_UNIFORM_FLOAT);

        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleLines(0, 0, widthUtils, windowHeight, WHITE);

        BeginShaderMode(shader);
        DrawTexture(texture, 0 + widthUtils, 0, WHITE);
        EndShaderMode();

        DrawRectangleRec(slider, WHITE);
        DrawCircle(c.cx, c.cy, c.radius, c.c);
        DrawText(std::to_string(static_cast<int>(redShift * 255)).c_str(), px + slider.width + elemPadd, py + slider.height / 2, 20, WHITE);
        EndDrawing();
    }

    UnloadShader(shader);
    UnloadImage(img);
    UnloadTexture(texture);
    CloseWindow();
}
