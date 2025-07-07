#pragma once

#include <Actor.h>

#include <CollisionListener.h>
#include <Types.h>

class AnimationRenderer;
class Game;

class Enemy final : public Actor, public CollisionListener
{
public:
	~Enemy() override = default;
	explicit Enemy(World& world, const EnemyType& type);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	auto on_collision_enter(const HitInfo& hit) -> void override;

	[[nodiscard]] auto get_type() const -> EnemyType { return m_enemy_type; }

private:
	EnemyType m_enemy_type = EnemyType::None;
	std::shared_ptr<AnimationRenderer> m_animation_component = {};

	auto build_enemy_frames_by_type() -> void;
};
