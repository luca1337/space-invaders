#include <Bullet.h>
#include <Input.h>
#include <Logger.h>
#include <Sprite.h>
#include <World.h>
#include <components/ShooterComponent.h>
#include <components/SpriteRenderer.h>
#include <components/Transform.h>

ShooterComponent::ShooterComponent(const std::shared_ptr<Actor>& owner) : Component(owner)
{
	if (!owner) { return; }

	m_bullet = std::make_shared<PlayerBullet>(owner->get_world());
	m_bullet->start();

	owner->get_world().add_actor(m_bullet);
}

void ShooterComponent::update(const float delta_time)
{
	Component::update(delta_time);

	const auto& bullet_transform = m_bullet->transform();
	const auto ow = owner().lock();

	if (!bullet_transform || !ow)
	{
		return;
	}

	if (const auto& win = ow->get_world().get_window(); Input::is_key_just_pressed(*win, SDLK_SPACE))
	{
		const auto& bullet_sprite_renderer_component = m_bullet->get_component<SpriteRenderer>();

		const auto& ship_transform = ow->transform();
		const auto& ship_sprite_renderer_component = ow->get_component<SpriteRenderer>();

		bullet_transform->set_position({ (ship_transform->get_position().x + ship_sprite_renderer_component->sprite_raw()->size().x * 0.5f) - bullet_sprite_renderer_component->sprite_raw()->size().x * 0.5f, ship_transform->get_position().y - 10.0f });

		m_bullet->m_velocity = { 0.0f, 250.0f };
	}
}
