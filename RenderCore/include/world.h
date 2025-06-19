#pragma once

#include <vector>
#include <memory>

class camera;
class window;
class actor;

class world final
{
public:
	~world() = default;
	world(const std::shared_ptr<window>& win, const std::shared_ptr<camera>& cam) : m_window{ win }, m_camera{ cam } {}
	auto setup() -> void;
	auto update() const -> void;
	auto add_actor(const std::shared_ptr<actor>& actor) -> void;

	[[nodiscard]] const auto& get_actors() const { return m_actors; }

	[[nodiscard]] const auto& get_window() const { return m_window; }
	[[nodiscard]] const auto& get_camera() const { return m_camera; }

private:
	std::shared_ptr<window> m_window = {};
	std::shared_ptr<camera> m_camera = {};
	std::vector<std::shared_ptr<actor>> m_actors = {};
};
