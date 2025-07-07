#pragma once

#include <components/Component.h>
#include <IRenderable.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct RenderContext;
class Sprite;

class AnimationRenderer final : public Component, public IRenderable
{
public:
    struct animation_clip_frame
    {
		std::shared_ptr<Sprite> frame_sprite = {};
        float duration = 0.1f;
    };

    struct animation_clip
    {
        std::vector<animation_clip_frame> frames = {};
    };

    explicit AnimationRenderer(const std::shared_ptr<Actor>& owner);

    auto update(float delta_time) -> void override;
    auto render(const RenderContext& ctx) -> void override;

    auto set_animation(const std::string& name, const animation_clip& clip) -> void;
    auto play(const std::string& name, bool loop = true) -> void;
    auto stop() -> void;

    [[nodiscard]] auto current_sprite() const -> std::shared_ptr<Sprite>;

private:
    std::unordered_map<std::string, animation_clip> m_animations = {};
    std::string m_current_animation = {};
    int m_current_frame = 0;
    float m_timer = 0.0f;
    bool m_is_playing = false;
    bool m_loop = true;
};
