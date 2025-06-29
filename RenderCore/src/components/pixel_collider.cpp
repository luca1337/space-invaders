#include <components/pixel_collider.h>

#include <actor.h>
#include <algorithm>
#include <components/box_collider.h>
#include <sprite.h>
#include <texture.h>
#include <components/transform.h>
#include <glm/glm.hpp>

pixel_collider::pixel_collider(const std::shared_ptr<actor>& owner, const std::shared_ptr<sprite>& sprite) : collider(owner), m_sprite(sprite) { }

auto pixel_collider::get_bounds() const -> glm::vec4
{
	const auto& transform = get_owner()->get_transform();
	const auto& pos = transform->get_position();
	const auto& size = m_sprite->size();

	return { pos.x, pos.y, size.x, size.y };
}

auto pixel_collider::collides_with(const collider& other, glm::vec2& impact_point) const -> bool
{
	const auto* other_pixel = dynamic_cast<const pixel_collider*>(&other);

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

			const auto ca = tex_a->get_pixel(ax, ay);
			const auto cb = tex_b->get_pixel(bx, by);

			if (ca.a > 0 && cb.a > 0)
			{
				impact_point = point;
				return true;
			}
		}
	}

	return false;
}
