#include <EnemyFormation.h>
#include <Game.h>
#include <ResourceManager.h>
#include <Shield.h>
#include <Ship.h>
#include <Window.h>
#include <World.h>
#include <components/SpriteRenderer.h>
#include <components/Transform.h>
#include <rendering/Camera.h>

auto Game::init() -> void
{
	constexpr auto window_width = 800;
	constexpr auto window_height = 600;
	constexpr auto window_depth = 32;
	constexpr auto gl_major_version = 4;
	constexpr auto gl_minor_version = 6;

	// -- Initialize SDL and OpenGL
	auto win = std::make_shared<Window>(window_width, window_height, window_depth, gl_major_version, gl_minor_version);
	win->set_post_processing_enabled(false);

	auto cam = std::make_shared<Camera>(0, win->props().width, win->props().height, 0);

	m_world = std::make_shared<World>(win, cam);
	m_world->setup();
}

auto Game::setup() -> void
{
	m_player = std::make_shared<Ship>(*m_world);
	m_player->start();

	m_world->add_actor(m_player);

	constexpr auto shield_num = 4;
	for (auto i = 0ul; i != shield_num; ++i)
	{
		constexpr auto spacing = 150.0f;

		const auto shield_actor = std::make_shared<Shield>(*m_world);
		shield_actor->start();

		const auto& sprite_rend = shield_actor->get_component<SpriteRenderer>();

		const auto shield_width = sprite_rend->sprite_raw()->size().x;
		const auto total_shields_width = shield_width * shield_num;

		constexpr auto total_spacing = spacing * (shield_num - 1);

		const auto total_used = total_shields_width + total_spacing;
		const auto padding = (m_world->get_window()->props().width - total_used) / 2.0f;

		const auto x = padding + i * (shield_width + spacing);
		const auto y = m_world->get_window()->props().height - sprite_rend->sprite_raw()->size().y * shield_num;

		const auto& transform = shield_actor->transform();

		transform->set_local_position({ x, y });
		m_world->add_actor(shield_actor);
	}

	const auto en_formation = std::make_shared<EnemyFormation>(*m_world);
	en_formation->start();
	m_world->add_actor(en_formation);
}

auto Game::tick() const -> void
{
	m_world->update();
}
