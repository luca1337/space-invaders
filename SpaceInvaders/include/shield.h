#pragma once

#include <actor.h>
#include <collision_listener.h>
#include <memory>
#include <sprite.h>

class sprite_renderer;
class game;

class shield final : public actor, public collision_listener
{
public:
	~shield() override = default;
	explicit shield(world& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	auto on_collision_enter(const hit_info& hit) -> void override;

	auto take_damage(const glm::vec2& impact_point) const -> void;

private:
	std::shared_ptr<sprite_renderer> m_sprite_component = {};
};