#pragma once

#include <Actor.h>
#include <CollisionListener.h>
#include <memory>
#include <Sprite.h>

class SpriteRenderer;
class Game;

class Shield final : public Actor, public CollisionListener
{
public:
	~Shield() override = default;
	explicit Shield(World& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

	auto on_collision_enter(const HitInfo& hit) -> void override;

	auto take_damage(const glm::vec2& impact_point) const -> void;

private:
	std::shared_ptr<SpriteRenderer> m_sprite_component = {};
};