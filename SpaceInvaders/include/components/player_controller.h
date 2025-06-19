#pragma once

#include <components/component.h>

class player_controller final : public component
{
public:
	explicit player_controller(const std::shared_ptr<actor>& owner);
	void update(float delta_time) override;
};

