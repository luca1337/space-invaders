#include <actor.h>
#include <collision_system.h>
#include <enums.h>
#include <resource_manager.h>
#include <window.h>
#include <world.h>
#include <rendering/shader.h>

auto world::setup() -> void
{
	// -- Register Resources
	const auto m_default_shader = std::make_shared<shader>("../RenderCore/resources/shaders/vertex_sprite.glsl", "../RenderCore/resources/shaders/fragment_sprite.glsl");
	resource_manager::register_resource<shader>(resource_params{ .m_resource_type = resource_type::shader, .m_name = "SpriteShader", .m_resource = m_default_shader });

	const auto m_particle_shader = std::make_shared<shader>("../RenderCore/resources/shaders/vertex_particle.glsl", "../RenderCore/resources/shaders/fragment_particle.glsl");
	resource_manager::register_resource<shader>(resource_params{ .m_resource_type = resource_type::shader, .m_name = "ParticleShader", .m_resource = m_particle_shader });
}

auto world::update() const -> void
{
	m_window->update([&](const float delta_time) {
		for (const auto& actor : m_actors)
		{
			if (actor->enabled())
			{
				actor->update(delta_time);
			}
		}

		collision_system::update_collisions(*this);

	});
}

auto world::add_actor(const std::shared_ptr<actor>& actor) -> void
{
	m_actors.push_back(actor);
}
