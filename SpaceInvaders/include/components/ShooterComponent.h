#pragma once

#include <components/Component.h>

class PlayerBullet;

class ShooterComponent final : public Component
{
public:
	explicit ShooterComponent(const std::shared_ptr<Actor>& owner);

	auto update(float delta_time) -> void override;

private:
	std::shared_ptr<PlayerBullet> m_bullet = {};
};
