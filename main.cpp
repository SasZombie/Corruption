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
#include <functional>

#include "colorManipulation.hpp"

struct Circle
{
    float cx;
    float cy;
    float radius;

    Color c;
};

struct Slider
{
    size_t maxValue;
    Circle pointer;
    Rectangle zone;

    int varLocation;
    float valValue = 0;
};

int main()
{
    // Vars
    Image img = LoadImage("test.png");

    const size_t imageWidth = img.width, imageHeight = img.height;
    constexpr size_t widthUtils = 200;
    constexpr size_t px = 15, py = 15;
    constexpr size_t elemPaddX = 30, elemPaddY = 3 * py;
    const size_t windowWidth = imageWidth + widthUtils, windowHeight = imageHeight;

    InitWindow(windowWidth, img.height, "Image Viewer");
    SetTargetFPS(60);

    // Shaders
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    RenderTexture2D target = LoadRenderTexture(windowWidth, imageHeight);

    Shader shader = LoadShader(0, "shaders/effects.fs");
    int redAddLoc = GetShaderLocation(shader, "redAdd");
    int greenAddLoc = GetShaderLocation(shader, "greenAdd");
    int blueAddLoc = GetShaderLocation(shader, "blueAdd");
    int redShiftXLoc = GetShaderLocation(shader, "redShiftX");
    int blueShiftXLoc = GetShaderLocation(shader, "blueShiftX");
    int greenShiftXLoc = GetShaderLocation(shader, "greenShiftX");
    
    int redShiftYLoc = GetShaderLocation(shader, "redShiftY");
    int blueShiftYLoc = GetShaderLocation(shader, "blueShiftY");
    int greenShiftYLoc = GetShaderLocation(shader, "greenShiftY");

    if (redAddLoc == -1 || greenAddLoc == -1 || blueAddLoc == -1 || blueShiftXLoc == -1 || greenShiftXLoc == -1 || redShiftXLoc == -1 
        || blueShiftYLoc == -1 || greenShiftYLoc == -1 || redShiftYLoc == -1)
    {
        std::cerr << "Shader uniform not found\n";
    }

    // Drawables
    constexpr size_t maxSliders = 9;
    std::array<Slider, maxSliders> sliders;

    Rectangle sliderPatR{px, py, 5, 255};
    Circle circlePatR{px + sliderPatR.width / 2, sliderPatR.y + sliderPatR.height, 10, RED};
    
    Rectangle sliderPatG{sliderPatR.x + sliderPatR.width + elemPaddX, py, 5, 255};
    Circle circlePatG{sliderPatG.x + sliderPatG.width / 2, sliderPatG.y + sliderPatG.height, 10, GREEN};

    Rectangle sliderPatB{sliderPatG.x + sliderPatG.width + elemPaddX, py, 5, 255};
    Circle circlePatB{sliderPatB.x + sliderPatB.width / 2, sliderPatB.y + sliderPatB.height, 10, BLUE};

    sliders[0] = {255, circlePatR, sliderPatR, redAddLoc};
    sliders[1] = {255, circlePatG, sliderPatG, greenAddLoc};
    sliders[2] = {255, circlePatB, sliderPatB, blueAddLoc};

    const float currentY = elemPaddY + sliderPatR.height + sliderPatR.y;
    
    //X axis
    Rectangle sliderShiftR{px, elemPaddY + sliderPatR.height + sliderPatR.y, 5, 500};
    Circle circleShiftR{px + sliderShiftR.width / 2, sliderShiftR.y + sliderShiftR.height, 10, RED};
    
    Rectangle sliderShiftG{sliderShiftR.x + sliderShiftR.width + elemPaddX, elemPaddY + sliderPatR.height + sliderPatR.y, 5, 500};
    Circle circleShiftG{sliderShiftG.x + sliderShiftG.width / 2, sliderShiftG.y + sliderShiftG.height, 10, GREEN};
    
    Rectangle sliderShiftB{sliderShiftG.x + sliderShiftG.width + elemPaddX, elemPaddY + sliderPatG.height + sliderPatG.y, 5, 500};
    Circle circleShiftB{sliderShiftB.x + sliderShiftB.width / 2, sliderShiftB.y + sliderShiftB.height, 10, BLUE};
   

    //Y axis
    Rectangle sliderShiftRY{sliderShiftB.x + sliderShiftB.width + elemPaddX, currentY, 5, 500};
    Circle circleShiftRY{sliderShiftRY.x + sliderShiftRY.width / 2, sliderShiftRY.y + sliderShiftRY.height, 10, RED};
   
    Rectangle sliderShiftGY{sliderShiftRY.x + sliderShiftRY.width + elemPaddX, currentY, 5, 500};
    Circle circleShiftGY{sliderShiftGY.x + sliderShiftGY.width / 2, sliderShiftGY.y + sliderShiftGY.height, 10, GREEN};
   
    Rectangle sliderShiftBY{sliderShiftGY.x + sliderShiftGY.width + elemPaddX, currentY, 5, 500};
    Circle circleShiftBY{sliderShiftBY.x + sliderShiftBY.width / 2, sliderShiftBY.y + sliderShiftBY.height, 10, BLUE};

    

    sliders[3] = {500, circleShiftR, sliderShiftR, redShiftXLoc};
    sliders[4] = {500, circleShiftG, sliderShiftG, greenShiftXLoc};
    sliders[5] = {500, circleShiftB, sliderShiftB, blueShiftXLoc};
    
    sliders[6] = {500, circleShiftRY, sliderShiftRY, redShiftYLoc};
    sliders[7] = {500, circleShiftGY, sliderShiftGY, greenShiftYLoc};
    sliders[8] = {500, circleShiftBY, sliderShiftBY, blueShiftYLoc};
    
    Slider *selected = nullptr;

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            const Vector2 mpos = GetMousePosition();

            for (auto &slider : sliders)
            {
                if (CheckCollisionPointCircle(mpos, {slider.pointer.cx, slider.pointer.cy}, slider.pointer.radius) || CheckCollisionPointRec(mpos, slider.zone))
                {
                    selected = &slider;
                }
            }
        }

        if (IsKeyPressed(KEY_S))
        {
            Image image = LoadImageFromTexture(target.texture);
            ExportImage(image, "output.png");
            UnloadImage(image); 
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            selected = nullptr;
        }

        if (selected)
        {
            const auto &[mx, my] = GetMousePosition();
            const size_t max = selected->zone.y + selected->zone.height;
            if (my > max)
            {
                selected->pointer.cy = max;
            }
            else if (my < selected->zone.y)
            {
                selected->pointer.cy = selected->zone.y;
            }
            else
            {
                selected->pointer.cy = my;
            }

            // std::cout << (selected->pointer.cy - selected->zone.y)  << '\n';
            selected->valValue = 1 - (selected->pointer.cy - selected->zone.y) / selected->maxValue;
            SetShaderValue(shader, selected->varLocation, &selected->valValue, SHADER_UNIFORM_FLOAT);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleLines(0, 0, widthUtils, imageHeight, WHITE);

        BeginTextureMode(target);
            BeginShaderMode(shader);
                DrawTexture(texture, widthUtils, 0, WHITE);
            EndShaderMode();
        EndTextureMode();

        DrawTextureRec(target.texture, {0, 0, static_cast<float>(windowWidth), -static_cast<float>(imageHeight)}, {0, 0}, WHITE);

        for (const auto &slider : sliders)
        {
            DrawRectangleRec(slider.zone, WHITE);
            DrawCircle(slider.pointer.cx, slider.pointer.cy, slider.pointer.radius, slider.pointer.c);
            //TODO: Make the px - 5 to be actually dynamic in regard to number of digits
            DrawText(std::to_string(static_cast<int>(slider.valValue * slider.maxValue)).c_str(), slider.zone.x - 5, slider.zone.y + slider.zone.height + py, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadShader(shader);
    UnloadTexture(texture);
    CloseWindow();
}
