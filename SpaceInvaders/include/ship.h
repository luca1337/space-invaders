#pragma once

#include <actor.h>
#include <memory>

#include <collision_listener.h>

class player_controller;
class sprite_renderer;
class bullet;
class game;

class ship final : public actor, public collision_listener
{
public:
	~ship() override = default;
	explicit ship(world& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	void on_collision_stay(const hit_info& hit) override;

private:
	std::shared_ptr<sprite_renderer> m_sprite_component = {};
};

