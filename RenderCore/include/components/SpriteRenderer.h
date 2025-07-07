#pragma once

#include <components/Component.h>
#include <IRenderable.h>
#include <memory>

class Sprite;

class SpriteRenderer final : public Component, public IRenderable
{
public:
	explicit SpriteRenderer(const std::shared_ptr<Actor>& owner, const std::shared_ptr<Sprite>& sprite);

	void update(float delta_time) override;
	auto render(const RenderContext& ctx) -> void override;

	[[nodiscard]] const auto& sprite_raw() const { return m_sprite; }
	[[nodiscard]] auto& sprite_raw() { return m_sprite; }

private:
	std::shared_ptr<Sprite> m_sprite;
};
