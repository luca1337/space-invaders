#include <actor.h>
#include <algorithm>
#include <components/collider.h>
#include <collision_listener.h>
#include <collision_system.h>
#include <hit_info.h>
#include <unordered_set>
#include <utility>
#include <world.h>

namespace
{
	struct actor_pair_hash
	{
		auto operator()(const std::pair<std::shared_ptr<actor>, std::shared_ptr<actor>>& p) const -> std::size_t
		{
			return std::hash<actor*>()(p.first.get()) ^ std::hash<actor*>()(p.second.get());
		}
	};

	using actor_collision_pair = std::pair<std::shared_ptr<actor>, std::shared_ptr<actor>>;
	using actor_collision_set = std::unordered_set<actor_collision_pair, actor_pair_hash>;
	auto active_collisions = actor_collision_set{};

	struct cell_coord
	{
		int x = {};
		int y = {};
		bool operator==(const cell_coord&) const = default;
	};

	struct cell_coord_hash
	{
		auto operator()(const cell_coord& c) const -> std::size_t
		{
			return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
		}
	};
}

auto collision_system::update_collisions(const world& w) -> void
{
	constexpr int cell_size = 32;
	const auto& actors = w.get_actors();

	// step 1: costruisco la griglia
	auto spatial_grid = std::unordered_map<cell_coord, std::vector<std::shared_ptr<actor>>, cell_coord_hash>{};

	for (const auto& actor : actors)
	{
		if (!actor->enabled()) continue;
		const auto& col = actor->get_component<collider>();
		if (!col) continue;

		const auto aabb = col->get_bounds();

		const int min_x = static_cast<int>(std::floor(aabb.x / cell_size));
		const int max_x = static_cast<int>(std::floor((aabb.x + aabb.z) / cell_size));
		const int min_y = static_cast<int>(std::floor(aabb.y / cell_size));
		const int max_y = static_cast<int>(std::floor((aabb.y + aabb.w) / cell_size));

		for (int x = min_x; x <= max_x; ++x)
		{
			for (int y = min_y; y <= max_y; ++y)
			{
				spatial_grid[{x, y}].push_back(actor);
			}
		}
	}

	// step 2: Collisioni solo all’interno delle celle evito di ciclare tutti gli actor partendo dal primo
	auto current_collisions = actor_collision_set{};

	for (const auto& [cell, cell_actors] : spatial_grid)
	{
		for (auto i = 0ul; i != cell_actors.size(); ++i)
		{
			const auto& a = cell_actors[i];
			const auto& a_col = a->get_component<collider>();
			if (!a_col || !a->enabled()) continue;

			for (auto j = i + 1; j != cell_actors.size(); ++j)
			{
				const auto& b = cell_actors[j];
				const auto& b_col = b->get_component<collider>();
				if (!b_col || !b->enabled()) continue;

				if (glm::vec2 impact_point; a_col->collides_with(*b_col, impact_point))
				{
					auto info = hit_info{};
					info.point = impact_point;

					const auto& key = std::minmax({ a, b });
					current_collisions.insert(key);

					const auto is_new = !active_collisions.contains(key);

					info.other = b->shared_from_this();
					if (const auto& l = std::dynamic_pointer_cast<collision_listener>(a))
					{
						if (is_new) l->on_collision_enter(info);
						else l->on_collision_stay(info);
					}

					info.other = a->shared_from_this();
					if (const auto& l = std::dynamic_pointer_cast<collision_listener>(b))
					{
						if (is_new) l->on_collision_enter(info);
						else l->on_collision_stay(info);
					}
				}
			}
		}
	}

	// Step 3: Collision exit
	for (const auto& prev : active_collisions)
	{
		if (!current_collisions.contains(prev))
		{
			auto& [a_ptr, b_ptr] = prev;

			auto info = hit_info{};
			info.other = b_ptr->shared_from_this();
			if (const auto& l = std::dynamic_pointer_cast<collision_listener>(a_ptr))
			{
				l->on_collision_exit(info);
			}

			info.other = a_ptr->shared_from_this();
			if (const auto& l = std::dynamic_pointer_cast<collision_listener>(b_ptr))
			{
				l->on_collision_exit(info);
			}
		}
	}

	active_collisions = std::move(current_collisions);
}
