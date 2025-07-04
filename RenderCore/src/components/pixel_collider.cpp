#include <actor.h>
#include <algorithm>
#include <resource_manager.h>
#include <sprite.h>
#include <texture.h>
#include <world.h>
#include <components/pixel_collider.h>
#include <components/transform.h>
#include <glm/glm.hpp>
#include <rendering/camera.h>
#include <rendering/render_context.h>
#include <rendering/shader.h>

pixel_collider::pixel_collider(const std::shared_ptr<actor>& owner, const std::shared_ptr<sprite>& s) : collider{ owner }, m_sprite{ s }
{
	enable_debug() = true;

	const auto& cam = owner->get_world().get_camera();
	const auto& sprite_shader = resource_manager::get_from_cache<shader>({ .m_resource_type = resource_type::shader, .m_name = "SpriteShader" });
	set_render_context({ .view = cam->get_view_matrix(), .projection = cam->get_projection_matrix(), .shader = sprite_shader.value(), .enable_debug = true });

	m_collider_sprite = std::make_shared<sprite>(m_sprite->sprite_texture()->get_width(), m_sprite->sprite_texture()->get_height(), green);
}

auto pixel_collider::get_bounds() const -> glm::vec4
{
	const auto owner = get_owner().lock();
	if (!owner || !m_sprite) { return {}; }

	const auto& transform = owner->get_transform();
	const auto& pos = transform->get_position();
	const auto& size = m_sprite->size();

	return { pos.x, pos.y, size.x, size.y };
}

auto pixel_collider::collides_with(const collider& other, glm::vec2& impact_point) const -> bool
{
	const auto& other_pixel = dynamic_cast<const pixel_collider*>(&other);

	if (!other_pixel) { return false; }

	const auto a_min = glm::vec2(get_bounds());
	const auto a_max = a_min + glm::vec2(get_bounds().z, get_bounds().w);
	const auto b_min = glm::vec2(other_pixel->get_bounds());
	const auto b_max = b_min + glm::vec2(other_pixel->get_bounds().z, other_pixel->get_bounds().w);

	const auto x_start = std::max(a_min.x, b_min.x);
	const auto x_end = std::min(a_max.x, b_max.x);
	const auto y_start = std::max(a_min.y, b_min.y);
	const auto y_end = std::min(a_max.y, b_max.y);

	if (x_end <= x_start || y_end <= y_start) { return false; }

	const auto& tex_a = m_sprite->sprite_texture();
	const auto& tex_b = other_pixel->m_sprite->sprite_texture();

	const auto tex_a_w = tex_a->get_width();
	const auto tex_a_h = tex_a->get_height();
	const auto tex_b_w = tex_b->get_width();
	const auto tex_b_h = tex_b->get_height();

	for (float y = y_start; y < y_end; ++y)
	{
		for (float x = x_start; x < x_end; ++x)
		{
			glm::vec2 point = { x, y };

			const glm::vec2 uv_a = (point - a_min) / (a_max - a_min);
			const glm::vec2 uv_b = (point - b_min) / (b_max - b_min);

			const auto ax = static_cast<int>(uv_a.x * tex_a_w);
			const auto ay = static_cast<int>((1.0f - uv_a.y) * tex_a_h);
			const auto bx = static_cast<int>(uv_b.x * tex_b_w);
			const auto by = static_cast<int>((1.0f - uv_b.y) * tex_b_h);

			if (ax < 0 || ay < 0 || ax >= tex_a_w || ay >= tex_a_h) continue;
			if (bx < 0 || by < 0 || bx >= tex_b_w || by >= tex_b_h) continue;

			const auto& ca = tex_a->get_pixel(ax, ay);
			const auto& cb = tex_b->get_pixel(bx, by);

			if (ca.a > 0 && cb.a > 0)
			{
				impact_point = point;
				return true;
			}
		}
	}

	return false;
}

auto pixel_collider::render(const render_context& ctx) -> void
{
	if (!enable_debug()) { return; }

	const auto& owner = get_owner().lock();

	if (!owner) { return; }

	const auto& transform = owner->get_transform();
	m_collider_sprite->render(ctx, *transform);
}