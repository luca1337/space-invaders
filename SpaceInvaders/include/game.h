#pragma once

#include <memory>
#include <vector>

class Enemy;
class Ship;
class Shield;
class World;

class Game final
{
public:
	~Game() = default;
    Game() = default;
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

	auto init() -> void;
	auto setup() -> void;
	auto tick() const -> void;

	[[nodiscard]] const auto& get_world() const { return m_world; }

private:
	std::shared_ptr<World> m_world = {};

	// -- Game entities
	std::vector<std::shared_ptr<Shield>> m_shields = {};
	std::shared_ptr<Ship> m_player = {};
	std::shared_ptr<Enemy> m_enemy = {};
};

