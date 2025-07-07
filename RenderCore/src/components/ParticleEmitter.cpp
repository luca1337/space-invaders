#include <Actor.h>
#include <exception>
#include <ResourceManager.h>
#include <World.h>
#include <components/ParticleEmitter.h>
#include <components/Transform.h>
#include <rendering/Camera.h>
#include <rendering/Shader.h>

ParticleEmitter::ParticleEmitter(const std::shared_ptr<Actor>& owner, const config& cfg) : Component{ owner }, m_config{ cfg }
{
	if (!owner)
	{
		throw std::exception("Particle emitter must be attached to a valid Actor.");
	}

	const auto& cam = owner->get_world().get_camera();
	const auto& particle_shader = ResourceManager::get_from_cache<Shader>({ .m_resource_type = ResourceType::shader, .m_name = "ParticleShader" });
	render_context() = {.view = cam->get_view_matrix(), .projection = cam->get_projection_matrix(), .shader = particle_shader.value()};

	const auto& transform = owner->transform();
	m_particle_system = std::make_unique<ParticleSystem>(
		transform->get_position(),
		cfg.amount,
		cfg.min_speed,
		cfg.max_speed,
		cfg.min_life_time,
		cfg.max_life_time
	);

	m_particle_system->set_emission_time_interval(cfg.emission_interval);
	m_particle_system->set_shape_type(cfg.shape);
	m_particle_system->expire() = cfg.expire;
	m_particle_system->set_on_finish_callback(cfg.on_finish_callback);
}

auto ParticleEmitter::start() -> void
{
	m_particle_system->reset();
	m_particle_system->emitting() = true;
}

auto ParticleEmitter::update(const float delta_time) -> void
{
	m_particle_system->update(delta_time);
}

auto ParticleEmitter::render(const RenderContext& ctx) -> void
{
	m_particle_system->render(ctx);
}
