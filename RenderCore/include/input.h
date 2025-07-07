#pragma once

#include <unordered_map>
#include <Window.h>
#include <SDL3/SDL.h>

#include <Export.h>

class RENDER_API Input
{
public:
    [[nodiscard]] static auto is_key_just_pressed(const Window& window, SDL_Keycode key) -> bool;
    [[nodiscard]] static auto is_key_pressed(const Window& window, SDL_Keycode key) -> bool;

private:
    static std::unordered_map<SDL_Keycode, bool> m_key_states;
};