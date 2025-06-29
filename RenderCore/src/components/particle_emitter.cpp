#include <actor.h>
#include <components/particle_emitter.h>
#include <components/transform.h>

#include <exception>

particle_emitter::particle_emitter(const std::shared_ptr<actor>& owner, const config& cfg) : component{ owner }, m_config{ cfg }
{
	if (!owner)
	{
		throw std::exception("Particle emitter must be attached to a valid actor.");
	}

	const auto& transform = owner->get_transform();
	m_particle_system = std::make_unique<particle_system>(
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

auto particle_emitter::start() -> void
{
	m_particle_system->reset();
	m_particle_system->emitting() = true;
}

auto particle_emitter::update(const float delta_time) -> void
{
	m_particle_system->update(delta_time);
}

auto particle_emitter::render(const render_context& ctx) const -> void
{
	m_particle_system->render(ctx);
}
