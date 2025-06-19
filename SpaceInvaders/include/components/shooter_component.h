#pragma once

#include <components/component.h>

class player_bullet;

class shooter_component final : public component
{
public:
	explicit shooter_component(const std::shared_ptr<actor>& owner);

	auto update(float delta_time) -> void override;

private:
	std::shared_ptr<player_bullet> m_bullet = {};
};
