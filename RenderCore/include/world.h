#pragma once

#include <vector>
#include <memory>

class Camera;
class Window;
class Actor;

class World final
{
public:
	~World() = default;
	World(const std::shared_ptr<Window>& win, const std::shared_ptr<Camera>& cam) : m_window{ win }, m_camera{ cam } {}
	auto setup() -> void;
	auto update() const -> void;
	auto add_actor(const std::shared_ptr<Actor>& actor) -> void;

	[[nodiscard]] const auto& get_actors() const { return m_actors; }

	[[nodiscard]] const auto& get_window() const { return m_window; }
	[[nodiscard]] const auto& get_camera() const { return m_camera; }

private:
	std::shared_ptr<Window> m_window = {};
	std::shared_ptr<Camera> m_camera = {};
	std::vector<std::shared_ptr<Actor>> m_actors = {};
};
