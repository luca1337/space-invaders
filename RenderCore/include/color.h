#pragma once

struct color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

static constexpr color gray = { 45, 45, 45, 0xFF };
static constexpr color black = { 0, 0, 0, 0xFF };
static constexpr color cyan = { 0, 255, 255, 0xFF };
static constexpr color yellow = { 255, 255, 0, 0xFF };
static constexpr color magenta = { 255, 0, 255, 0xFF };
static constexpr color green = { 0, 255, 0, 0xFF };
static constexpr color red = { 255, 0, 0, 0xFF };
static constexpr color blue = { 0, 0, 255, 0xFF };
static constexpr color orange = { 255, 165, 0, 0xFF };
static constexpr color black_no_alpha = { 0, 0, 0, 0 };