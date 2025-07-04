#include <shield.h>

#include <enums.h>
#include <random.h>
#include <resource_manager.h>
#include <sprite.h>
#include <components/sprite_renderer.h>
#include <texture.h>
#include <components/transform.h>
#include <world.h>
#include <rendering/camera.h>
#include <rendering/shader.h>

#include <components/pixel_collider.h>

shield::shield(world& w) : actor{ w } { }

void shield::start()
{
	actor::start();

	const auto shield_sprite = std::make_shared<sprite>("assets/Invaders/space__0008_ShieldFull.png");
	m_sprite_component = add_component<sprite_renderer>(shield_sprite);

	add_component<pixel_collider>(shield_sprite);
}

void shield::update(const float delta_time)
{
	actor::update(delta_time);

}

auto shield::on_collision_enter(const hit_info& hit) -> void
{
	collision_listener::on_collision_enter(hit);
}

auto shield::take_damage(const glm::vec2& impact_point) const -> void
{
	const auto& transform = get_transform();
	const auto& sprite = m_sprite_component->sprite_raw();

	const auto& local = impact_point - transform->get_local_position();
	const auto uv = local / sprite->size();

	const auto& tex = sprite->sprite_texture();
	const int tex_width = tex->get_width();
	const int tex_height = tex->get_height();

	const int center_x = static_cast<int>(uv.x * tex_width);
	const int center_y = tex_height - 1 - static_cast<int>(uv.y * tex_height);

	constexpr int radius = 15;

	for (int dy = -radius; dy <= radius; ++dy)
	{
		for (int dx = -radius; dx <= radius; ++dx)
		{
			const int x = center_x + dx;
			const int y = center_y + dy;

			if (x < 0 || y < 0 || x >= tex_width || y >= tex_height) { continue; }

			if (const float distance = glm::length(glm::vec2{ dx, dy }); distance <= radius)
			{
				if (const auto probability = 1.0f - distance / static_cast<float>(radius); rng::generate_random_number(0.0f, 1.0f) < probability)
				{
					tex->set_pixel(x, y, black_no_alpha);
				}
			}
		}
	}

	tex->update();
}
