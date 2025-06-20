#pragma once

#include <memory>
#include <map>

#include <actor.h>
#include <types.h>

class enemy;
class world;

struct enemy_group
{
	enemy_type type = {};
	std::vector<std::shared_ptr<enemy>> members = {};
};

class enemy_formation final : public actor
{
	using enemy_map = std::map<enemy_type, std::vector<enemy_group>>;

public:
	explicit enemy_formation(world& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

private:
	enemy_map m_groups = {};
};
