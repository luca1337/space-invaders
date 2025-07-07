#include <Actor.h>
#include <ResourceManager.h>
#include <Sprite.h>
#include <utility>
#include <World.h>
#include <components/AnimationRenderer.h>
#include <components/Transform.h>
#include <rendering/Camera.h>
#include <rendering/RenderContext.h>
#include <rendering/Shader.h>

AnimationRenderer::AnimationRenderer(const std::shared_ptr<Actor>& owner) : Component{ owner } 
{
	const auto& cam = owner->get_world().get_camera();
	const auto& sprite_shader = ResourceManager::get_from_cache<Shader>({ .m_resource_type = ResourceType::shader, .m_name = "SpriteShader" });
	render_context() = { .view = cam->get_view_matrix(), .projection = cam->get_projection_matrix(), .shader = sprite_shader.value() };
}

void AnimationRenderer::update(const float delta_time)
{
	if (!m_is_playing || m_current_animation.empty()) { return; }

	const auto& [frames] = m_animations[m_current_animation];
	if (frames.empty()) { return; }

	m_timer += delta_time;

	if (const auto& [frame_sprite, duration] = frames[m_current_frame]; m_timer >= duration)
	{
		m_timer = {};
		++m_current_frame;

		if (std::cmp_greater_equal(m_current_frame, frames.size()))
		{
			if (m_loop)
			{
				m_current_frame = {};
			}
			else
			{
				m_is_playing = false;
				m_current_frame = static_cast<int>(frames.size()) - 1;
			}
		}
	}
}

void AnimationRenderer::render(const RenderContext& ctx)
{
	if (const auto& sprite = current_sprite())
	{
		if (const auto& ow = owner().lock(); ow)
		{
			const auto& transform = ow->transform();
			sprite->render(ctx, *transform);
		}
	}
}

auto AnimationRenderer::set_animation(const std::string& name, const AnimationClip& clip) -> void
{
	m_animations[name] = clip;
}

auto AnimationRenderer::play(const std::string& name, const bool loop) -> void
{
	if (!m_animations.contains(name)) { return; }

	m_current_animation = name;
	m_current_frame = 0;
	m_timer = 0.0f;
	m_is_playing = true;
	m_loop = loop;
}

auto AnimationRenderer::stop() -> void
{
	m_is_playing = false;
	m_current_frame = 0;
	m_timer = 0.0f;
}

auto AnimationRenderer::current_sprite() const -> std::shared_ptr<Sprite>
{
	const auto& [frames] = m_animations.at(m_current_animation);
	if (frames.empty()) { return {}; }

	return frames[m_current_frame].frame_sprite;
}
