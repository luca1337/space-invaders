#pragma once

#include <actor.h>
#include <collision_listener.h>
#include <memory>
#include <glm/vec2.hpp>

class particle_emitter;
class sprite_renderer;
class animation_renderer;

class player_bullet final : public actor, public collision_listener
{
public:
	explicit player_bullet(world& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	auto on_collision_enter(const hit_info& hit) -> void override;

	glm::vec2 m_velocity = {};

private:
	std::shared_ptr<particle_emitter> m_impact_fx = {};
	std::shared_ptr<sprite_renderer> m_sprite_component = {};
};

class enemy_bullet final : public actor, public collision_listener
{
public:
	explicit enemy_bullet(world& w);

	auto update(float delta_time) -> void override;

	auto on_collision_enter(const hit_info& hit) -> void override;

private:
	std::shared_ptr<animation_renderer> m_animation_component = {};
};