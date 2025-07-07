#pragma once

#include <Actor.h>
#include <memory>

#include <CollisionListener.h>

class PlayerController;
class SpriteRenderer;
class bullet;
class Game;

class Ship final : public Actor, public CollisionListener
{
public:
	~Ship() override = default;
	explicit Ship(World& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	void on_collision_stay(const HitInfo& hit) override;

private:
	std::shared_ptr<SpriteRenderer> m_sprite_component = {};
};

