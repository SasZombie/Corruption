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


static int getRandomInt(int min, int max) noexcept
{
    static std::random_device rd;  
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist;

    dist.param(std::uniform_int_distribution<int>::param_type(min, max));

    return dist(gen);
}

static std::tuple<int, int, int> getRandomColor() noexcept
{
    static std::random_device rd;  
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(0, 1);

    return std::make_tuple(255 * dist(gen), 255 * dist(gen), 255 * dist(gen));
}

struct glitchEffect{
    int chosenLine;
    int startingPoint;
    int length;
    int thiccness;
    int endPoint;
    int R, G, B;
};

int main()
{
    //Components 3 = RGB | 4 = RGBA
    int width, height, components;
    unsigned char *data = stbi_load("test.jpeg", &width, &height, &components, 0);
    if (!data)
    {
        std::cerr << "Failed to load image\n";
        return 1;
    }

    const int lines = 20;
    const int maxLength = 200;
    std::vector<glitchEffect> effects;
    
    for(size_t i = 0; i < lines; ++i)
    {
        int chosenLine = getRandomInt(10, 890);
        int startingPoint = getRandomInt(10, 1580);
        int remaining = width - startingPoint;
        remaining = std::min(remaining, maxLength);
        int length = getRandomInt(std::min(50, remaining), std::max(50, remaining));
        int endPoint = length + startingPoint;
        int thiccness = getRandomInt(2, 6);
        const auto& [R, G, B] = getRandomColor();

        effects.emplace_back(chosenLine, startingPoint, length, thiccness, endPoint, R, G, B);
    }
    
    const int offsetGreen = 3;
    const int offsetBlue = 3;


    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            int i = (y * width + x) * components;
            for(const auto& line: effects)
            {

                if(line.R && y >= line.chosenLine - line.thiccness && y <= line.chosenLine + line.thiccness && x > line.startingPoint && x < line.endPoint)
                {
                    data[i + 0] = line.R;
                    data[i + 1] = 0;  
                    data[i + 2] = 0;  
                }
                else if(line.G && y >= line.chosenLine - line.thiccness + offsetGreen && y <= line.chosenLine + line.thiccness + offsetGreen && x > line.startingPoint + offsetGreen && x < line.endPoint + offsetGreen)
                {
                    data[i + 0] = 0;
                    data[i + 1] = line.G;  
                    data[i + 2] = 0;  
                    
                }else if(line.B && y >= line.chosenLine - line.thiccness + offsetBlue && y <= line.chosenLine + line.thiccness + offsetBlue && x > line.startingPoint + offsetBlue && x < line.endPoint + offsetBlue)
                {
                    data[i + 0] = 0;
                    data[i + 1] = 0;  
                    data[i + 2] = line.B;  
                    
                }
            }
        }

    if (!stbi_write_png("output.jpeg", width, height, components, data, width * components))
    {
        std::cerr << "Could not write image\n";
        return 1;
    }

    free(data);
}