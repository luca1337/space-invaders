#pragma once

#include <components/Component.h>
#include <IRenderable.h>
#include <rendering/ParticleSystem.h>
#include <memory>
#include <functional>

class ParticleEmitter final : public Component, public IRenderable
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
		ShapeType shape = ShapeType::circle;
		bool expire = {};
		std::function<void()> on_finish_callback = {};
	};

	explicit ParticleEmitter(const std::shared_ptr<Actor>& owner, const config& cfg);

	auto start() -> void override;
	auto update(float delta_time) -> void override;
	auto render(const RenderContext& ctx) -> void override;

	[[nodiscard]] const auto& get_emitter()  { return m_particle_system; }
	[[nodiscard]] const auto& get_config() { return m_config; }

private:
	std::unique_ptr<ParticleSystem> m_particle_system = {};
	config m_config = {};
};