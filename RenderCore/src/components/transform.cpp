#include <components/transform.h>

transform::transform(const std::shared_ptr<actor>& owner) : component{ owner } {}

auto transform::set_parent(const std::shared_ptr<transform>& parent) -> void
{
	if (parent == m_parent.lock()) { return; }

	if (const auto old_parent = m_parent.lock())
	{
		old_parent->remove_child(std::static_pointer_cast<transform>(shared_from_this()));
	}

	if (parent)
	{
		parent->add_child(std::static_pointer_cast<transform>(shared_from_this()));
		m_local_position = m_position - parent->m_position;
		m_previous_parent_rotation = parent->m_rotation;
		m_previous_parent_scale = parent->m_scale;
	}

	m_parent = parent;
}

auto transform::set_position(const glm::vec2& pos) -> void
{
	m_position = pos;

	if (const auto parent = m_parent.lock())
	{
		m_local_position = m_position - parent->m_position;
	}

	for (const auto& child : m_children)
	{
		child->update_child_position();
	}
}

auto transform::set_local_position(const glm::vec2& pos) -> void
{
	m_local_position = pos;

	if (const auto parent = m_parent.lock())
	{
		m_position = parent->m_position + m_local_position;
	}
	else
	{
		m_position = m_local_position;
	}

	for (const auto& child : m_children)
	{
		child->update_child_position();
	}
}

auto transform::set_rotation(const float rot) -> void
{
	m_rotation = rot;

	for (const auto& child : m_children)
	{
		child->update_child_rotation();
	}
}

auto transform::set_euler_degrees(const float deg) -> void
{
	set_rotation(glm::radians(deg));
}

auto transform::set_scale(const glm::vec2& s) -> void
{
	m_scale = s;

	for (const auto& child : m_children)
	{
		child->update_child_scale();
	}
}

auto transform::update_child_position() -> void
{
	if (const auto parent = m_parent.lock())
	{
		m_position = parent->m_position + m_local_position;
	}
}

auto transform::update_child_rotation() -> void
{
	if (const auto parent = m_parent.lock())
	{
		const auto delta = parent->m_rotation - m_previous_parent_rotation;
		m_rotation += delta;

		const float c = glm::cos(-delta);
		const float s = glm::sin(-delta);
		const glm::mat2 rot_matrix(c, -s, s, c);

		m_local_position = rot_matrix * m_local_position;
		m_position = parent->m_position + m_local_position;

		m_previous_parent_rotation = parent->m_rotation;
	}
}

auto transform::update_child_scale() -> void
{
	if (const auto parent = m_parent.lock())
	{
		const auto ratio = parent->m_scale / m_previous_parent_scale;
		m_scale *= ratio;
		m_position = parent->m_position + m_local_position * ratio;
		m_previous_parent_scale = parent->m_scale;
	}
}

auto transform::add_child(const std::shared_ptr<transform>& child) -> void
{
	if (std::ranges::find(m_children, child) == m_children.end())
	{
		m_children.emplace_back(child);
	}
}

auto transform::remove_child(const std::shared_ptr<transform>& child) -> void
{
	std::erase(m_children, child);
}
