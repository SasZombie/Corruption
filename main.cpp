#include <print>
#include <iostream>
#include <random>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wmissing-declarations"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#pragma GCC diagnostic pop

[[maybe_unused]] static void interestingPattern(int width, int height, int components, unsigned char *R, unsigned char *G, unsigned char *B, unsigned char *data) noexcept
{
    int index = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int i = (y * width + x) * components;
            data[i + 1] = data[i + 1] + R[index]; // G
            data[i + 2] = data[i + 2] + G[index]; // B
            ++index;
        }
    }
}

static std::tuple<unsigned char *, unsigned char *, unsigned char *> toComponents(int width, int height, int components, const unsigned char *data) noexcept
{
    unsigned char *R = new unsigned char[width * height];
    unsigned char *G = new unsigned char[width * height];
    unsigned char *B = new unsigned char[width * height];

    size_t index = 0;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int i = (y * width + x) * components;
            R[index] = data[i + 0]; // R
            G[index] = data[i + 1]; // G
            B[index] = data[i + 2]; // B
            ++index;
        }
    }

    return std::make_tuple(R, G, B);
}

static unsigned char *toGrayscale(int width, int height, int components, const unsigned char *data) noexcept
{
    unsigned char *gray = new unsigned char[width * height];

    size_t index = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int i = (y * width + x) * components;

            gray[index] = static_cast<unsigned char>(0.299f * data[i + 0] + 0.587f * data[i + 1] + 0.114f * data[i + 2]);
            ++index;
        }
    }

    return gray;
}

[[maybe_unused]] static void shiftRed(int width, int height, int components, unsigned char *R, unsigned char *data, size_t index, size_t maxIndex) noexcept
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int i = (y * width + x) * components;

            data[i + 0] = R[index];
            ++index;

            if (index == maxIndex)
            {
                index = 0;
            }
        }
    }
}

static unsigned char *toMask(int width, int height, const unsigned char *gray, const int gx[3][3], const int gy[3][3]) noexcept
{
    unsigned char *mask = new unsigned char[width * height];

    for (int y = 1; y < height - 1; ++y)
    {
        for (int x = 1; x < width - 1; ++x)
        {
            int gxSum = 0;
            int gySum = 0;

            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    unsigned char pixel = gray[(y + dy) * width + (x + dx)];

                    gxSum = gx[dy + 1][dx + 1] * pixel;
                    gySum = gy[dy + 1][dx + 1] * pixel;
                }
            }

            int magnitude = std::sqrt(gxSum * gxSum + gySum * gySum);
            mask[y * width + x] = (magnitude > 70) ? 255 : 0;
        }
    }

    return mask;
}

int main()
{
    // Components 3 = RGB | 4 = RGBA
    int width, height, components;
    unsigned char *data = stbi_load("test.png", &width, &height, &components, 0);
    if (!data)
    {
        std::cerr << "Failed to load image\n";
        return 1;
    }
    else
    {
        std::print("Loaded image with {} x {}, {} components\n", width, height, components);
    }

    const auto &[R, G, B] = toComponents(width, height, components, data);
    const unsigned char *gray = toGrayscale(width, height, components, data);

    const int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    const int gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    const unsigned char *mask = toMask(width, height, gray, gx, gy);

    stbi_write_png("mask.png", width, height, 1, mask, width);

    // size_t index = 20;
    // const size_t maxIndex = width * height;
    // shiftRed(width, height, components, R, data, index, maxIndex);
    
    int redShift = 20;
    int greenShift = -10;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int idx = y * width + x;
            if (mask[idx] == 255)
            {
                int i = idx * components;
                int redShiftedIndex = (idx + redShift) % (width * height);
                int greenShiftedIndex = (idx + greenShift) % (width * height);

                data[i + 0] = R[redShiftedIndex]; 
                data[i + 1] = G[greenShiftedIndex]; 
            }
        }
    }

    if (!stbi_write_png("output.png", width, height, components, data, width * components))
    {
        std::cerr << "Could not write image\n";
        return 1;
    }

    free(data);
    delete[] R;
    delete[] G;
    delete[] B;
    delete[] gray;
    delete[] mask;
}