#pragma once

#include <concepts>
#include <memory>
#include <ranges>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <optional>

#include <rendering/render_context.h>

class world;
class transform;
class component;

template <typename T>
concept is_component = std::derived_from<T, component>;

class actor : public std::enable_shared_from_this<actor>
{
public:
	virtual ~actor() = default;
	explicit actor(world& w) : m_world{ w } {}
	explicit actor(world& w, std::string name);

	virtual auto start() -> void;
	virtual auto update(float delta_time) -> void;

	template <is_component T, typename... Args>
	auto add_component(Args&&... args) -> std::shared_ptr<T>
	{
		if (auto existing = get_component<T>())
		{
			return existing;
		}

		auto self = shared_from_this();
		auto new_comp = std::make_shared<T>(self, std::forward<Args>(args)...);
		m_components[std::type_index(typeid(T))] = new_comp;
		return new_comp;
	}

	template <is_component T>
	auto get_exact_component() const -> std::shared_ptr<T>
	{
		if (const auto it = m_components.find(std::type_index(typeid(T))); it != m_components.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return {};
	}

	template <is_component T>
	auto get_component() const -> std::shared_ptr<T>
	{
		for (const auto& comp : m_components | std::views::values)
		{
			if (auto casted = std::dynamic_pointer_cast<T>(comp))
			{
				return casted;
			}
		}
		return {};
	}

	template <is_component T>
	auto has_component() const { return get_component<T>() != nullptr; }

	[[nodiscard]] auto& enabled() { return m_enabled; }
	[[nodiscard]] const auto& enabled() const { return m_enabled; }

	[[nodiscard]] auto& get_transform() { return m_transform; }
	[[nodiscard]] const auto& get_transform() const { return m_transform; }

	[[nodiscard]] const auto& get_name() const { return m_name; }

	[[nodiscard]] const auto& get_world() const { return m_world; }
	[[nodiscard]] auto& get_world() { return m_world; }

private:
	world& m_world;
	std::string m_name = "default";
	std::unordered_map<std::type_index, std::shared_ptr<component>> m_components;
	std::shared_ptr<transform> m_transform = {};
	bool m_enabled = true;
};
