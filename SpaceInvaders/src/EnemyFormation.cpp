#include <Enemy.h>
#include <EnemyFormation.h>
#include <Window.h>
#include <World.h>
#include <components/AnimationRenderer.h>
#include <components/Transform.h>
#include <glm/vec2.hpp>

EnemyFormation::EnemyFormation(World& w) : Actor{ w }
{
	constexpr int rows = 5;
	constexpr int cols = 11;
	constexpr float spacing_x = 40.0f;

	constexpr auto total_width = cols * spacing_x;
	const auto window_width = static_cast<float>(get_world().get_window()->props().width);
	const auto start_x = (window_width - total_width) / 2.0f;

	for (auto row = 0ul; row != rows; ++row)
	{
		auto type = EnemyType{};

		if (row == 0) { type = EnemyType::squid; }
		else if (row <= 2) { type = EnemyType::crab; }
		else { type = EnemyType::octopus; }

		auto group = EnemyGroup{ .type = type };

		for (auto col = 0ul; col != cols; ++col)
		{
			constexpr float start_y = 100.0f;
			constexpr float spacing_y = 40.0f;

			auto en = std::make_shared<Enemy>(get_world(), type);
			en->start();

			const auto& anim_rend = en->get_component<AnimationRenderer>();
			anim_rend->stop();

			const auto& enemy_transform = en->transform();
			enemy_transform->set_position(glm::vec2{ start_x + col * spacing_x,start_y + row * spacing_y });

			get_world().add_actor(en);

			group.members.push_back(en);
		}

		m_groups[type].push_back(std::move(group));
	}
}

void EnemyFormation::start()
{
	Actor::start();
}

void EnemyFormation::update(float delta_time)
{
}