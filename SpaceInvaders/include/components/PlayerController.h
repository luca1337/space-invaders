#pragma once

#include <components/Component.h>

class PlayerController final : public Component
{
public:
	explicit PlayerController(const std::shared_ptr<Actor>& owner);
	void update(float delta_time) override;
};

