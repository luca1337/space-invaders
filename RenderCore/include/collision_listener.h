#pragma once

struct hit_info;

class collision_listener
{
public:
	virtual ~collision_listener() = default;

	virtual void on_collision_enter(const hit_info& hit) {}
	virtual void on_collision_stay(const hit_info& hit) {}
	virtual void on_collision_exit(const hit_info& hit) {}
};