#include <actor.h>
#include <sprite.h>
#include <components/sprite_renderer.h>
#include <components/transform.h>

sprite_renderer::sprite_renderer(const std::shared_ptr<actor>& owner, const std::shared_ptr<sprite>& sprite) : component{ owner }, m_sprite(sprite)
{
}

void sprite_renderer::update(float delta_time)
{
}

void sprite_renderer::render(const render_context& ctx)
{
	if (!m_sprite) { return; }

	if (const auto& owner = get_owner().lock(); owner)
	{
		const auto& transform = owner->get_transform();
		m_sprite->render(ctx, *transform);
	}
}
