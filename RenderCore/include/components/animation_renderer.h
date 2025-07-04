#pragma once

#include <components/component.h>
#include <irenderable.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct render_context;
class sprite;

class animation_renderer final : public component, public irenderable
{
public:
    struct animation_clip_frame
    {
        std::shared_ptr<sprite> frame_sprite;
        float duration = 0.1f;
    };

    struct animation_clip
    {
        std::vector<animation_clip_frame> frames;
    };

    explicit animation_renderer(const std::shared_ptr<actor>& owner);

    auto update(float delta_time) -> void override;
    auto render(const render_context& ctx) -> void override;

    auto set_animation(const std::string& name, const animation_clip& clip) -> void;
    auto play(const std::string& name, bool loop = true) -> void;
    auto stop() -> void;

    [[nodiscard]] auto current_sprite() const -> std::shared_ptr<sprite>;

private:
    std::unordered_map<std::string, animation_clip> m_animations = {};
    std::string m_current_animation = {};
    int m_current_frame = 0;
    float m_timer = 0.0f;
    bool m_is_playing = false;
    bool m_loop = true;
};
