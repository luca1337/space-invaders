#pragma once

class world;

namespace collision_system
{
	auto update_collisions(const world& w) -> void;
}
