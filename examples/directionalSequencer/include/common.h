#pragma once

#include <stdint.h>

constexpr uint8_t GridSizeX = 8;
constexpr uint8_t GridSizeY = 4;

struct CellCoords {
	uint8_t x;
	uint8_t y;
};

struct Point {
	uint8_t x;
	uint8_t y;
};

struct Bounds {
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
};

template <typename T>
inline constexpr T min(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
inline constexpr T max(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
inline constexpr T clamp(T val, T lo, T hi) {
    return (val < lo) ? lo : (val > hi) ? hi : val;
}