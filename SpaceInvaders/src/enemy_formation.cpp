#include <enemy_formation.h>

#include <components/animation_renderer.h>
#include <components/transform.h>
#include <window.h>
#include <world.h>
#include <glm/vec2.hpp>

#include <enemy.h>

enemy_formation::enemy_formation(world& w) : actor{ w }
{
	constexpr int rows = 5;
	constexpr int cols = 11;
	constexpr float spacing_x = 40.0f;

	constexpr auto total_width = cols * spacing_x;
	const auto window_width = static_cast<float>(get_world().get_window()->props().width);
	const auto start_x = (window_width - total_width) / 2.0f;

	for (auto row = 0ul; row != rows; ++row)
	{
		auto type = enemy_type{};

		if (row == 0) { type = enemy_type::squid; }
		else if (row <= 2) { type = enemy_type::crab; }
		else { type = enemy_type::octopus; }

		auto group = enemy_group{ .type = type };

		for (auto col = 0ul; col != cols; ++col)
		{
			constexpr float start_y = 100.0f;
			constexpr float spacing_y = 40.0f;

			auto en = std::make_shared<enemy>(get_world(), type);
			en->start();

			const auto& anim_rend = en->get_component<animation_renderer>();
			anim_rend->stop();

			const auto& enemy_transform = en->get_transform();
			enemy_transform->set_position(glm::vec2{ start_x + col * spacing_x,start_y + row * spacing_y });

			get_world().add_actor(en);

			group.members.push_back(en);
		}

		m_groups[type].push_back(std::move(group));
	}
}

void enemy_formation::start()
{
	actor::start();
}

void enemy_formation::update(float delta_time)
{
}