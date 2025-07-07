#include <Actor.h>
#include <IRenderable.h>
#include <ranges>
#include <components/Transform.h>

Actor::Actor(World& w, std::string name) : m_world{ w }, m_name{ std::move(name) } {}

auto Actor::start() -> void
{
	m_transform = add_component<Transform>();
}

auto Actor::update(const float delta_time) -> void
{
	for (const auto& comp : m_components | std::views::values)
	{
		if (!comp->enabled())
		{
			continue;
		}

		comp->update(delta_time);

		if (const auto& renderable_component = std::dynamic_pointer_cast<IRenderable>(comp))
		{
			if (auto& render_context = comp->render_context())
			{
				render_context->enable_debug = comp->debug();
				renderable_component->render(render_context.value());
			}
		}
	}
}