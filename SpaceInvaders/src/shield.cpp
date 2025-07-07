#include <Random.h>
#include <ResourceManager.h>
#include <Shield.h>
#include <Sprite.h>
#include <Texture.h>
#include <components/PixelCollider.h>
#include <components/SpriteRenderer.h>
#include <components/Transform.h>
#include <rendering/Camera.h>

Shield::Shield(World& w) : Actor{ w } { }

void Shield::start()
{
	Actor::start();

	const auto shield_sprite = std::make_shared<Sprite>("assets/Invaders/space__0008_ShieldFull.png");
	m_sprite_component = add_component<SpriteRenderer>(shield_sprite);

	add_component<PixelCollider>(shield_sprite);
}

void Shield::update(const float delta_time)
{
	Actor::update(delta_time);

}

auto Shield::on_collision_enter(const HitInfo& hit) -> void
{
	CollisionListener::on_collision_enter(hit);
}

auto Shield::take_damage(const glm::vec2& impact_point) const -> void
{
	const auto& tf = transform();
	const auto& sprite = m_sprite_component->sprite_raw();

	const auto& local = impact_point - tf->get_local_position();
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
				if (const auto probability = 1.0f - distance / static_cast<float>(radius); Rng::generate_random_number(0.0f, 1.0f) < probability)
				{
					tex->set_pixel(x, y, black_no_alpha);
				}
			}
		}
	}

	tex->update();
}
