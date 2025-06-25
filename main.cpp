#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#include "raylib.h"

#pragma GCC diagnostic pop

#include <iostream>
#include <functional>

#include "shaderEmbeded.hpp"
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

static int getImageChannelCount(int format)
{
    switch (format)
    {
    case PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
        return 1;
    case PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
        return 2;
    case PIXELFORMAT_UNCOMPRESSED_R5G6B5:
    case PIXELFORMAT_UNCOMPRESSED_R8G8B8:
        return 3;
    case PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:
    case PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:
    case PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
        return 4;
    default:
        return -1; // Unknown or compressed format
    }
}

int main()
{
    // Vars
    int imgComponents = 0;
    size_t windowWidth = 800, windowHeight = 600;
    size_t imgWidth = 800, imgHeight = 600;
    constexpr size_t widthUtils = 230;
    constexpr size_t px = 15, py = 15;
    constexpr size_t elemPaddX = 30, elemPaddY = 3 * py;
    unsigned char *mask = nullptr;

    const int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    const int gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    InitWindow(windowWidth, windowHeight, "Image Viewer");
    SetTargetFPS(60);

    // Shaders
    Texture2D texture, maskTexture;
    RenderTexture2D target;

#ifdef bakedShaders
    Shader shader = LoadShaderFromMemory(NULL, effects_shader);
#else
    Shader shader = LoadShader(0, "shaders/effects.fs");
#endif

    int redAddLoc = GetShaderLocation(shader, "redAdd");
    int greenAddLoc = GetShaderLocation(shader, "greenAdd");
    int blueAddLoc = GetShaderLocation(shader, "blueAdd");
    int redShiftXLoc = GetShaderLocation(shader, "redShiftX");
    int blueShiftXLoc = GetShaderLocation(shader, "blueShiftX");
    int greenShiftXLoc = GetShaderLocation(shader, "greenShiftX");

    int redShiftYLoc = GetShaderLocation(shader, "redShiftY");
    int blueShiftYLoc = GetShaderLocation(shader, "blueShiftY");
    int greenShiftYLoc = GetShaderLocation(shader, "greenShiftY");

    int maskTextLoc = GetShaderLocation(shader, "maskTexture");
    int maskEffectOnLoc = GetShaderLocation(shader, "on");
    int displayMaskLoc = GetShaderLocation(shader, "displayMask");

    if (redAddLoc == -1 || greenAddLoc == -1 || blueAddLoc == -1 || blueShiftXLoc == -1 || greenShiftXLoc == -1 || redShiftXLoc == -1 || blueShiftYLoc == -1 || greenShiftYLoc == -1 || redShiftYLoc == -1 || maskTextLoc == -1 || maskEffectOnLoc == -1 || displayMaskLoc == -1)
    {
        std::cerr << "Not all unifrom shaders found";
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

    Rectangle toggleMaskEffect{sliderPatB.x + elemPaddX, sliderPatB.y, 30, 30};
    Rectangle toggleSeeMask{sliderPatB.x + elemPaddX, toggleMaskEffect.y + toggleMaskEffect.height + elemPaddY, 30, 30};

    sliders[0] = {255, circlePatR, sliderPatR, redAddLoc};
    sliders[1] = {255, circlePatG, sliderPatG, greenAddLoc};
    sliders[2] = {255, circlePatB, sliderPatB, blueAddLoc};

    const float currentY = elemPaddY + sliderPatR.height + sliderPatR.y;

    // X axis
    Rectangle sliderShiftR{px, elemPaddY + sliderPatR.height + sliderPatR.y, 5, 500};
    Circle circleShiftR{px + sliderShiftR.width / 2, sliderShiftR.y + sliderShiftR.height, 10, RED};

    Rectangle sliderShiftG{sliderShiftR.x + sliderShiftR.width + elemPaddX, elemPaddY + sliderPatR.height + sliderPatR.y, 5, 500};
    Circle circleShiftG{sliderShiftG.x + sliderShiftG.width / 2, sliderShiftG.y + sliderShiftG.height, 10, GREEN};

    Rectangle sliderShiftB{sliderShiftG.x + sliderShiftG.width + elemPaddX, elemPaddY + sliderPatG.height + sliderPatG.y, 5, 500};
    Circle circleShiftB{sliderShiftB.x + sliderShiftB.width / 2, sliderShiftB.y + sliderShiftB.height, 10, BLUE};

    // Y axis
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

    bool hasImage = false;
    bool useMask = false;
    bool displayMask = false;

    while (!WindowShouldClose())
    {
        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            if (droppedFiles.count > 0)
            {
                const char *filePath = droppedFiles.paths[0];

                Image img = LoadImage(filePath);

                imgWidth = img.width;
                imgHeight = img.height;

                windowHeight = img.height;
                windowWidth = img.width + widthUtils;
                imgComponents = getImageChannelCount(img.format);

                if (hasImage)
                {
                    UnloadTexture(texture);
                }

                texture = LoadTextureFromImage(img);
                target = LoadRenderTexture(windowWidth, windowHeight);
                unsigned char *gray = toGrayscale(img.width, img.height, imgComponents, static_cast<unsigned char *>(img.data));
                mask = toMask(img.width, img.height, gray, gx, gy);

                Image maskImg = {
                    .data = mask,
                    .width = img.width,
                    .height = img.height,
                    .mipmaps = 1,
                    .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE};

                maskTexture = LoadTextureFromImage(maskImg);

                SetShaderValueTexture(shader, maskTextLoc, maskTexture);
                UnloadImage(img);

                delete[] gray;
                hasImage = true;
                SetWindowSize(windowWidth, windowHeight);
            }

            UnloadDroppedFiles(droppedFiles);
        }

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

            if (CheckCollisionPointRec(mpos, toggleMaskEffect))
            {
                useMask = !useMask;
                float maskVal = useMask ? 1.f : 0.f;
                SetShaderValue(shader, maskEffectOnLoc, &maskVal, SHADER_UNIFORM_FLOAT);
            }

            if (CheckCollisionPointRec(mpos, toggleSeeMask))
            {
                displayMask = !displayMask;
                float maskVal = displayMask ? 1.f : 0.f;
                SetShaderValue(shader, displayMaskLoc, &maskVal, SHADER_UNIFORM_FLOAT);
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

            selected->valValue = 1 - (selected->pointer.cy - selected->zone.y) / selected->maxValue;
            SetShaderValue(shader, selected->varLocation, &selected->valValue, SHADER_UNIFORM_FLOAT);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (!hasImage)
        {
            DrawText("Drag and drop an image file here", 10, 10, 20, DARKGRAY);
        }
        else
        {

            DrawRectangleLines(0, 0, widthUtils, windowHeight, WHITE);

            BeginTextureMode(target);
            BeginShaderMode(shader);
            DrawTexture(texture, widthUtils, 0, WHITE);
            EndShaderMode();
            EndTextureMode();

            DrawTextureRec(target.texture, {0, 0, static_cast<float>(windowWidth), -static_cast<float>(windowHeight)}, {0, 0}, WHITE);

            for (const auto &slider : sliders)
            {
                DrawRectangleRec(slider.zone, WHITE);
                DrawCircle(slider.pointer.cx, slider.pointer.cy, slider.pointer.radius, slider.pointer.c);
                // TODO: Make the px - 5 to be actually dynamic in regard to number of digits
                DrawText(std::to_string(static_cast<int>(slider.valValue * slider.maxValue)).c_str(), slider.zone.x - 5, slider.zone.y + slider.zone.height + py, 20, WHITE);
            }

            DrawRectangleRec(toggleMaskEffect, RED);
            DrawRectangleRec(toggleSeeMask, BLUE);
        }

        EndDrawing();
    }

    UnloadShader(shader);
    if (hasImage)
    {
        UnloadTexture(texture);
    }
    CloseWindow();
    delete[] mask;
}
