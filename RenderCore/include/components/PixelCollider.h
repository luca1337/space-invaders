#pragma once

#include <components/Collider.h>
#include <IRenderable.h>
#include <memory>

class Sprite;

class PixelCollider final : public Collider, public IRenderable
{
public:
	explicit PixelCollider(const std::shared_ptr<Actor>& owner, const std::shared_ptr<Sprite>& sprite);

	auto get_bounds() const->glm::vec4 override;
	auto collides_with(const Collider& other, glm::vec2& impact_point) const -> bool override;

	// Inherited via irenderable
	auto render(const RenderContext& ctx) -> void override;
private:
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Sprite> m_collider_sprite;
};
#pragma once
