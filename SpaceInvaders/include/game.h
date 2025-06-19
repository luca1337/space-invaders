#pragma once

#include <memory>
#include <vector>

class enemy;
class ship;
class shield;
class world;

class game final
{
public:
	~game() = default;
    game() = default;
    game(const game&) = delete;
    game(game&&) = delete;
    game& operator=(const game&) = delete;
    game& operator=(game&&) = delete;

	auto init() -> void;
	auto setup() -> void;
	auto tick() const -> void;

	[[nodiscard]] const auto& get_world() const { return m_world; }

private:
	std::shared_ptr<world> m_world = {};

	// -- Game entities
	std::vector<std::shared_ptr<shield>> m_shields = {};
	std::shared_ptr<ship> m_player = {};
	std::shared_ptr<enemy> m_enemy = {};
};

