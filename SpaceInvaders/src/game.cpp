#include <game.h>

#include <resource_manager.h>
#include <components/sprite_renderer.h>
#include <components/transform.h>
#include <window.h>
#include <world.h>
#include <rendering/camera.h>
#include <rendering/shader.h>

#include <enemy_formation.h>
#include <shield.h>
#include <ship.h>

auto game::init() -> void
{
	constexpr auto window_width = 800;
	constexpr auto window_height = 600;
	constexpr auto window_depth = 32;
	constexpr auto gl_major_version = 4;
	constexpr auto gl_minor_version = 6;

	// -- Initialize SDL and OpenGL
	auto win = std::make_shared<window>(window_width, window_height, window_depth, gl_major_version, gl_minor_version);
	win->set_post_processing_enabled(false);

	auto cam = std::make_shared<camera>(0, win->props().width, win->props().height, 0);

	m_world = std::make_shared<world>(win, cam);
	m_world->setup();
}

auto game::setup() -> void
{
	m_player = std::make_shared<ship>(*m_world);
	m_player->start();

	m_world->add_actor(m_player);

	constexpr auto shield_num = 4;
	for (auto i = 0ul; i != shield_num; ++i)
	{
		constexpr auto spacing = 150.0f;

		const auto shield_actor = std::make_shared<shield>(*m_world);
		shield_actor->start();

		const auto& sprite_rend = shield_actor->get_component<sprite_renderer>();

		const auto shield_width = sprite_rend->sprite_raw()->size().x;
		const auto total_shields_width = shield_width * shield_num;

		constexpr auto total_spacing = spacing * (shield_num - 1);

		const auto total_used = total_shields_width + total_spacing;
		const auto padding = (m_world->get_window()->props().width - total_used) / 2.0f;

		const auto x = padding + i * (shield_width + spacing);
		const auto y = m_world->get_window()->props().height - sprite_rend->sprite_raw()->size().y * shield_num;

		const auto& transform = shield_actor->get_transform();

		transform->set_local_position({ x, y });
		m_world->add_actor(shield_actor);
	}

	const auto en_formation = std::make_shared<enemy_formation>(*m_world);
	en_formation->start();
	m_world->add_actor(en_formation);
}

auto game::tick() const -> void
{
	m_world->update();
}
