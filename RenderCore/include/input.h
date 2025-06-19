#pragma once

#include <unordered_map>
#include <window.h>
#include <SDL3/SDL.h>

#include <export.h>

class RENDER_API input
{
public:
    [[nodiscard]] static auto is_key_just_pressed(const window& window, SDL_Keycode key) -> bool;
    [[nodiscard]] static auto is_key_pressed(const window& window, SDL_Keycode key) -> bool;

private:
    static std::unordered_map<SDL_Keycode, bool> m_key_states;
};