#pragma once

#include <collider.h>
#include <memory>

class sprite;

class pixel_collider final : public collider
{
public:
	pixel_collider(const std::shared_ptr<actor>& owner, const std::shared_ptr<sprite>& sprite);

	auto get_bounds() const->glm::vec4 override;
	auto collides_with(const collider& other, glm::vec2& impact_point) const -> bool override;

private:
	std::shared_ptr<sprite> m_sprite;
};
#pragma once
