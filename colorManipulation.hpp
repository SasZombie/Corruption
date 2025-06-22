#pragma once
#include <tuple>
#include <cstddef>


void interestingPattern(int width, int height, int components, unsigned char *R, unsigned char *G, unsigned char *B, unsigned char *data) noexcept;
std::tuple<unsigned char *, unsigned char *, unsigned char *> toComponents(int width, int height, int components, const unsigned char *data) noexcept;
unsigned char *toGrayscale(int width, int height, int components, const unsigned char *data) noexcept;
void shiftRed(int width, int height, int components, unsigned char *R, unsigned char *data, size_t index, size_t maxIndex) noexcept;
unsigned char *toMask(int width, int height, const unsigned char *gray, const int gx[3][3], const int gy[3][3]) noexcept;
