#include <enemy.h>
#include <logger.h>
#include <resource_manager.h>
#include <sprite.h>
#include <types.h>
#include <components/animation_renderer.h>
#include <components/pixel_collider.h>
#include <rendering/shader.h>

namespace
{
	const std::vector<std::string> squid_frames = {
		"assets/Invaders/space__0000_A1.png",
		"assets/Invaders/space__0001_A2.png",
	};

	const std::vector<std::string> crab_frames = {
		"assets/Invaders/space__0002_B1.png",
		"assets/Invaders/space__0003_B2.png",
	};

	const std::vector<std::string> octopus_frames = {
		"assets/Invaders/space__0004_C1.png",
		"assets/Invaders/space__0005_C2.png",
	};

	const std::vector<std::string> ufo_frames = {
		"assets/Invaders/space__0007_UFO.png",
	};
}

enemy::enemy(world& w, const enemy_type& type) : actor{ w }, m_enemy_type{ type }
{

}

void enemy::build_enemy_frames_by_type()
{
	const std::vector<std::string>* frames = {};

	switch (m_enemy_type)
	{
	case squid:   frames = &squid_frames; break;
	case crab:    frames = &crab_frames; break;
	case octopus: frames = &octopus_frames; break;
	case ufo:     frames = &ufo_frames; break;
	case none:    return;
	default: break;
	}

	if (!frames) return;

	auto anim_clip = animation_renderer::animation_clip{};

	for (const auto& path : *frames)
	{
		const auto enemy_sprite_frame = std::make_shared<sprite>(path);
		anim_clip.frames.push_back({ .frame_sprite = enemy_sprite_frame, .duration = 0.7f });
		add_component<pixel_collider>(enemy_sprite_frame);
	}

	m_animation_component = add_component<animation_renderer>();
	m_animation_component->set_animation("idle", anim_clip);
	m_animation_component->play("idle", true);
}

void enemy::start()
{
	actor::start();

	build_enemy_frames_by_type();
}

void enemy::update(const float delta_time)
{
	actor::update(delta_time);
}

void enemy::on_collision_enter(const hit_info& hit)
{
	collision_listener::on_collision_enter(hit);

	LOG_INFO("Enemy on_collision_enter with {}", static_cast<int>(m_enemy_type));
}