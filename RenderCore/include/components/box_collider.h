#pragma once

#include <components/collider.h>

class sprite;

class box_collider final : public collider
{
public:
	explicit box_collider(const std::shared_ptr<actor>& owner);

	auto get_bounds() const->glm::vec4 override;
	auto collides_with(const collider& other, glm::vec2& impact_point) const -> bool override;

private:
	std::shared_ptr<sprite> m_sprite;
};
