#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(float left, float right, float bottom, float top);

	[[nodiscard]] const auto& position() const { return m_position; }
	auto& position() { return m_position; }

	[[nodiscard]] auto rotation() const -> float { return m_rotation; }
	auto rotation(const float rotation) -> void { m_rotation = rotation; recalculate_view_matrix(); }

	auto set_zoom(const float zoom) { m_zoom = zoom; }

	[[nodiscard]] auto get_projection_matrix() const -> glm::mat4 { return m_projection_matrix; }
	[[nodiscard]] auto get_view_matrix() const -> glm::mat4 { return m_view_matrix; }
	[[nodiscard]] auto get_view_projection_matrix() const -> glm::mat4 { return m_view_projection_matrix; }

private:
	auto recalculate_view_matrix() -> void;

private:
	glm::mat4 m_projection_matrix = {};
	glm::mat4 m_view_matrix = {};
	glm::mat4 m_view_projection_matrix = {};

	glm::vec3 m_position = {};
	float m_rotation = {};
	float m_zoom = 1.0f;
};