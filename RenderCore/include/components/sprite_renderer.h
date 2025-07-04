#pragma once

#include <components/component.h>
#include <irenderable.h>
#include <memory>

class sprite;

class sprite_renderer final : public component, public irenderable
{
public:
	explicit sprite_renderer(const std::shared_ptr<actor>& owner, const std::shared_ptr<sprite>& sprite);

	void update(float delta_time) override;
	auto render(const render_context& ctx) -> void override;

	[[nodiscard]] const auto& sprite_raw() const { return m_sprite; }
	[[nodiscard]] auto& sprite_raw() { return m_sprite; }

private:
	std::shared_ptr<sprite> m_sprite;
};
