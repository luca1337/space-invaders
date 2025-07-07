#include <rendering/Camera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

Camera::Camera(const float left, const float right, const float bottom, const float top)
	: m_projection_matrix{ glm::ortho(left, right, bottom, top, -1.0f, 1.0f) }, m_view_matrix{ glm::mat4(1.0f) }
{
	recalculate_view_matrix();
}

auto Camera::recalculate_view_matrix() -> void
{
	const auto translation = glm::translate(glm::mat4(1.0f), m_position);
	const auto rotation = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3{ 0.0f, 0.0f, 1.0f });
	const auto scale = glm::scale(glm::mat4(1.0f), glm::vec3{ m_zoom, m_zoom, 1.0f });
	const auto transform = translation * rotation * scale;

	m_view_matrix = glm::inverse(transform);
	m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}