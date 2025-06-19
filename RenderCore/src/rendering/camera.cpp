#include <rendering/camera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

camera::camera(const float left, const float right, const float bottom, const float top)
	: m_projection_matrix{ glm::ortho(left, right, bottom, top, -1.0f, 1.0f) }, m_view_matrix{ glm::mat4(1.0f) }
{ }

auto camera::recalculate_view_matrix() -> void
{
	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3{ 0.0f, 0.0f, 1.0f });

	m_view_matrix = glm::inverse(transform);
	m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}