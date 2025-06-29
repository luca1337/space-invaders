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
		auto operator()(const std::pair<actor*, actor*>& p) const -> std::size_t
		{
			return std::hash<actor*>()(p.first) ^ std::hash<actor*>()(p.second);
		}
	};

	using actor_collision_pair = std::pair<actor*, actor*>;
	using actor_collision_set = std::unordered_set<actor_collision_pair, actor_pair_hash>;

	actor_collision_set active_collisions = {};
}

auto collision_system::update_collisions(const world& w) -> void
{
	const auto& actors = w.get_actors();

	auto current_collisions = actor_collision_set{};

	for (auto i = 0ul; i != actors.size(); ++i)
	{
		const auto a = actors[i];
		const auto a_col = a->get_component<collider>();

		if (!a_col || !a->enabled()) { continue; }

		for (auto j = i + 1ul; j != actors.size(); ++j)
		{
			const auto b = actors[j];
			const auto b_col = b->get_component<collider>();

			if (!b_col || !b->enabled()) { continue; }

			if (glm::vec2 impact_point; a_col->collides_with(*b_col, impact_point))
			{
				auto info = hit_info{};
				info.point = impact_point;

				const auto key = std::minmax({ a.get(), b.get() });
				current_collisions.insert(key);

				const bool is_new = !active_collisions.contains(key);

				info.other = b;
				if (auto* l = dynamic_cast<collision_listener*>(a.get()))
				{
					if (is_new) l->on_collision_enter(info);
					else l->on_collision_stay(info);
				}

				info.other = a;
				if (auto* l = dynamic_cast<collision_listener*>(b.get()))
				{
					if (is_new) l->on_collision_enter(info);
					else l->on_collision_stay(info);
				}
			}
		}
	}

	// Collision exit
	for (const auto& prev : active_collisions)
	{
		if (!current_collisions.contains(prev))
		{
			auto& [a_ptr, b_ptr] = prev;

			auto info = hit_info{};
			info.other = b_ptr->shared_from_this();
			if (auto* l = dynamic_cast<collision_listener*>(a_ptr))
			{
				l->on_collision_exit(info);
			}

			info.other = a_ptr->shared_from_this();
			if (auto* l = dynamic_cast<collision_listener*>(b_ptr))
			{
				l->on_collision_exit(info);
			}
		}
	}

	active_collisions = std::move(current_collisions);
}
