#include <Bullet.h>
#include <Input.h>
#include <ResourceManager.h>
#include <Ship.h>
#include <Sprite.h>
#include <World.h>
#include <components/PixelCollider.h>
#include <components/PlayerController.h>
#include <components/ShooterComponent.h>
#include <components/SpriteRenderer.h>
#include <components/Transform.h>
#include <rendering/Camera.h>

Ship::Ship(World& w) : Actor{ w } {}

void Ship::start()
{
	Actor::start();

	const auto player_sprite = std::make_shared<Sprite>("assets/Invaders/space__0006_Player.png");

	m_sprite_component = add_component<SpriteRenderer>(player_sprite);

	const auto a = add_component<PixelCollider>(player_sprite);
	a->debug() = true;

	add_component<PlayerController>();
	add_component<ShooterComponent>();

	const auto& tf = transform();
	const auto& window = get_world().get_window();

	tf->set_local_position(glm::vec2{ window->props().width / 2.0f - m_sprite_component->sprite_raw()->size().x * 0.5f, window->props().height - m_sprite_component->sprite_raw()->size().y * 4.0f });
}

void Ship::update(const float delta_time)
{
	Actor::update(delta_time);
}

void Ship::on_collision_stay(const HitInfo& hit)
{
}

