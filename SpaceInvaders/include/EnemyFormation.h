#pragma once

#include <memory>
#include <map>

#include <Actor.h>
#include <Types.h>

class Enemy;
class World;

struct EnemyGroup
{
	EnemyType type = {};
	std::vector<std::shared_ptr<Enemy>> members = {};
};

class EnemyFormation final : public Actor
{
	using enemy_map = std::map<EnemyType, std::vector<EnemyGroup>>;

public:
	explicit EnemyFormation(World& w);

	auto start() -> void override;
	auto update(float delta_time) -> void override;

private:
	enemy_map m_groups = {};
};
