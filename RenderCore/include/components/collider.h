#pragma once

#include <components/Component.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Collider : public Component
{
public:
	using Component::Component;

	virtual auto collides_with(const Collider& other, glm::vec2& impact_point) const -> bool = 0;
	virtual auto get_bounds() const->glm::vec4 = 0; // x, y, w, h
};
