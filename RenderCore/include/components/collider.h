#pragma once

#include <components/component.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class collider : public component
{
public:
	using component::component;

	virtual auto collides_with(const collider& other, glm::vec2& impact_point) const -> bool = 0;
	virtual auto get_bounds() const->glm::vec4 = 0; // x, y, w, h

	auto& enable_debug() { return m_enable_debug; }

protected:
	bool m_enable_debug = {};
};
