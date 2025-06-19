#pragma once

#include <collider.h>

class box_collider final : public collider
{
public:
	using collider::collider;

	auto get_bounds() const->glm::vec4 override;
	auto collides_with(const collider& other, glm::vec2& impact_point) const -> bool override;
};
