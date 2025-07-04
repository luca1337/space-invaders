#include <actor.h>
#include <irenderable.h>
#include <ranges>
#include <components/transform.h>

actor::actor(world& w, std::string name) : m_world{ w }, m_name{ std::move(name) } {}

auto actor::start() -> void
{
	m_transform = add_component<transform>();
}

auto actor::update(const float delta_time) -> void
{
	for (const auto& comp : m_components | std::views::values)
	{
		comp->update(delta_time);

		if (const auto& renderable_component = std::dynamic_pointer_cast<irenderable>(comp))
		{
			renderable_component->render(comp->get_render_context().value());
		}
	}
}