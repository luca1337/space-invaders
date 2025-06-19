#pragma once

#include <actor.h>

#include <collision_listener.h>
#include <enums.h>

class animation_renderer;
class game;

class enemy final : public actor, public collision_listener
{
public:
	~enemy() override = default;
	explicit enemy(world& w, const enemy_type& type);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	auto on_collision_enter(const hit_info& hit) -> void override;

	[[nodiscard]] auto get_type() const -> enemy_type { return m_enemy_type; }

private:
	enemy_type m_enemy_type = enemy_type::none;
	std::shared_ptr<animation_renderer> m_animation_component = {};

	auto build_enemy_frames_by_type() -> void;
};
