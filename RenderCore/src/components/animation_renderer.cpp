#include <actor.h>
#include <components/animation_renderer.h>
#include <sprite.h>
#include <texture.h>
#include <components/transform.h>

animation_renderer::animation_renderer(const std::shared_ptr<actor>& owner) : component{ owner } {}

void animation_renderer::update(const float delta_time)
{
	if (!m_is_playing || m_current_animation.empty()) { return; }

	const auto& [frames] = m_animations[m_current_animation];
	if (frames.empty()) { return; }

	m_timer += delta_time;

	if (const auto& [frame_sprite, duration] = frames[m_current_frame]; m_timer >= duration)
	{
		m_timer = 0.0f;
		++m_current_frame;

		if (m_current_frame >= static_cast<int>(frames.size()))
		{
			if (m_loop)
			{
				m_current_frame = 0;
			}
			else
			{
				m_is_playing = false;
				m_current_frame = static_cast<int>(frames.size()) - 1;
			}
		}
	}
}

void animation_renderer::render(const render_context& ctx)
{
	if (const auto sprite = current_sprite())
	{
		if (const auto owner = get_owner().lock(); owner)
		{
			const auto& transform = owner->get_transform();
			sprite->render(ctx, *transform);
		}
	}
}

auto animation_renderer::set_animation(const std::string& name, const animation_clip& clip) -> void
{
	m_animations[name] = clip;
}

auto animation_renderer::play(const std::string& name, const bool loop) -> void
{
	if (!m_animations.contains(name)) { return; }

	m_current_animation = name;
	m_current_frame = 0;
	m_timer = 0.0f;
	m_is_playing = true;
	m_loop = loop;
}

auto animation_renderer::stop() -> void
{
	m_is_playing = false;
	m_current_frame = 0;
	m_timer = 0.0f;
}

auto animation_renderer::current_sprite() const -> std::shared_ptr<sprite>
{
	const auto& [frames] = m_animations.at(m_current_animation);
	if (frames.empty()) { return nullptr; }

	return frames[m_current_frame].frame_sprite;
}
