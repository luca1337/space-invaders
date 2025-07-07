#include <Actor.h>
#include <CollisionSystem.h>
#include <Enums.h>
#include <ResourceManager.h>
#include <Window.h>
#include <World.h>
#include <rendering/Shader.h>

auto World::setup() -> void
{
	// -- Register Resources
	const auto m_default_shader = std::make_shared<Shader>("../RenderCore/resources/shaders/vertex_sprite.glsl", "../RenderCore/resources/shaders/fragment_sprite.glsl");
	ResourceManager::register_resource<Shader>(ResourceParams{ .m_resource_type = ResourceType::shader, .m_name = "SpriteShader", .m_resource = m_default_shader });

	const auto m_particle_shader = std::make_shared<Shader>("../RenderCore/resources/shaders/vertex_particle.glsl", "../RenderCore/resources/shaders/fragment_particle.glsl");
	ResourceManager::register_resource<Shader>(ResourceParams{ .m_resource_type = ResourceType::shader, .m_name = "ParticleShader", .m_resource = m_particle_shader });
}

auto World::update() const -> void
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

auto World::add_actor(const std::shared_ptr<Actor>& actor) -> void
{
	m_actors.push_back(actor);
}
