#include <components/shooter_component.h>

#include <input.h>
#include <world.h>

#include <bullet.h>
#include <logger.h>
#include <components/transform.h>

#include <components/sprite_renderer.h>

#include <sprite.h>

shooter_component::shooter_component(const std::shared_ptr<actor>& owner) : component(owner)
{
	if (!owner) { return; }

	m_bullet = std::make_shared<player_bullet>(owner->get_world());
	m_bullet->start();

	owner->get_world().add_actor(m_bullet);
}

void shooter_component::update(const float delta_time)
{
	component::update(delta_time);

	const auto& bullet_transform = m_bullet->get_transform();
	const auto owner = get_owner().lock();

	if (!bullet_transform || !owner)
	{
		return;
	}

	if (const auto& win = owner->get_world().get_window(); input::is_key_just_pressed(*win, SDLK_SPACE))
	{
		const auto& bullet_sprite_renderer_component = m_bullet->get_component<sprite_renderer>();

		const auto& ship_transform = owner->get_transform();
		const auto& ship_sprite_renderer_component = owner->get_component<sprite_renderer>();

		bullet_transform->set_position({ (ship_transform->get_position().x + ship_sprite_renderer_component->sprite_raw()->size().x * 0.5f) - bullet_sprite_renderer_component->sprite_raw()->size().x * 0.5f, ship_transform->get_position().y - 10.0f });

		m_bullet->m_velocity = { 0.0f, 250.0f };
	}
}
