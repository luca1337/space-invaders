#pragma once

#include <components/collider.h>
#include <irenderable.h>
#include <memory>

class sprite;

class pixel_collider final : public collider, public irenderable
{
public:
	pixel_collider(const std::shared_ptr<actor>& owner, const std::shared_ptr<sprite>& sprite);

	auto get_bounds() const->glm::vec4 override;
	auto collides_with(const collider& other, glm::vec2& impact_point) const -> bool override;

	// Inherited via irenderable
	auto render(const render_context& ctx) -> void override;
private:
	std::shared_ptr<sprite> m_sprite;
	std::shared_ptr<sprite> m_collider_sprite;
};
#pragma once
