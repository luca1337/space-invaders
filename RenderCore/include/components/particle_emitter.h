#pragma once

#include <components/component.h>
#include <rendering/particle_system.h>
#include <memory>
#include <functional>

class particle_emitter final : public component
{
public:
	struct config
	{
		unsigned amount = 100;
		float min_speed = 20.f;
		float max_speed = 100.f;
		float min_life_time = 0.5f;
		float max_life_time = 1.5f;
		float emission_interval = 0.004f;
		shape_type shape = shape_type::circle;
		bool expire = {};
		std::function<void()> on_finish_callback = {};
	};

	explicit particle_emitter(const std::shared_ptr<actor>& owner, const config& cfg);

	auto start() -> void override;
	auto update(float delta_time) -> void override;
	auto render(const render_context& ctx) const -> void;

	[[nodiscard]] const auto& get_emitter()  { return m_particle_system; }
	[[nodiscard]] const auto& get_config() { return m_config; }

private:
	std::unique_ptr<particle_system> m_particle_system = {};
	config m_config = {};
};