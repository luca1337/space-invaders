#pragma once

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

static constexpr Color gray = { 45, 45, 45, 0xFF };
static constexpr Color black = { 0, 0, 0, 0xFF };
static constexpr Color cyan = { 0, 255, 255, 0xFF };
static constexpr Color yellow = { 255, 255, 0, 0xFF };
static constexpr Color magenta = { 255, 0, 255, 0xFF };
static constexpr Color green = { 0, 255, 0, 0xFF };
static constexpr Color red = { 255, 0, 0, 0xFF };
static constexpr Color blue = { 0, 0, 255, 0xFF };
static constexpr Color orange = { 255, 165, 0, 0xFF };
static constexpr Color black_no_alpha = { 0, 0, 0, 0 };