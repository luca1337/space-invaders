#pragma once

#include <Actor.h>
#include <CollisionListener.h>
#include <memory>
#include <glm/vec2.hpp>

class ParticleEmitter;
class SpriteRenderer;
class AnimationRenderer;

class PlayerBullet final : public Actor, public CollisionListener
{
public:
	explicit PlayerBullet(World& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	auto on_collision_enter(const HitInfo& hit) -> void override;

	glm::vec2 m_velocity = {};

private:
	std::shared_ptr<ParticleEmitter> m_impact_fx = {};
	std::shared_ptr<SpriteRenderer> m_sprite_component = {};
};

class EnemyBullet final : public Actor, public CollisionListener
{
public:
	explicit EnemyBullet(World& w);

	auto update(float delta_time) -> void override;

	auto on_collision_enter(const HitInfo& hit) -> void override;

private:
	std::shared_ptr<AnimationRenderer> m_animation_component = {};
};