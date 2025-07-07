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
    struct AnimationClipFrame
    {
		std::shared_ptr<Sprite> frame_sprite = {};
        float duration = 0.1f;
    };

    struct AnimationClip
    {
        std::vector<AnimationClipFrame> frames = {};
    };

    ~AnimationRenderer() override = default;
    explicit AnimationRenderer(const std::shared_ptr<Actor>& owner);

    auto update(float delta_time) -> void override;
    auto render(const RenderContext& ctx) -> void override;

    auto set_animation(const std::string& name, const AnimationClip& clip) -> void;
    auto play(const std::string& name, bool loop = true) -> void;
    auto stop() -> void;

    [[nodiscard]] auto current_sprite() const -> std::shared_ptr<Sprite>;

private:
    std::unordered_map<std::string, AnimationClip> m_animations = {};
    std::string m_current_animation = {};
    int m_current_frame = {};
    float m_timer = {};
    bool m_is_playing = {};
    bool m_loop = true;
};
