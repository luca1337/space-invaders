#include <actor.h>
#include <resource_manager.h>
#include <sprite.h>
#include <world.h>
#include <components/sprite_renderer.h>
#include <components/transform.h>
#include <rendering/camera.h>
#include <rendering/render_context.h>
#include <rendering/shader.h>

sprite_renderer::sprite_renderer(const std::shared_ptr<actor>& owner, const std::shared_ptr<sprite>& sprite) : component{ owner }, m_sprite{ sprite }
{
	const auto& cam = owner->get_world().get_camera();
	const auto& sprite_shader = resource_manager::get_from_cache<shader>({ .m_resource_type = resource_type::shader, .m_name = "SpriteShader" });
	set_render_context({ .view = cam->get_view_matrix(), .projection = cam->get_projection_matrix(), .shader = sprite_shader.value() });
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
