#pragma once

#include <components/component.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <algorithm>

class transform final : public component
{
public:
    explicit transform(const std::shared_ptr<actor>& owner);

    auto set_parent(const std::shared_ptr<transform>& parent) -> void;
    [[nodiscard]] auto get_parent() const -> std::shared_ptr<transform> { return m_parent.lock(); }
    [[nodiscard]] auto get_children() const -> const std::vector<std::shared_ptr<transform>>& { return m_children; }

    auto set_position(const glm::vec2& pos) -> void;
    [[nodiscard]] auto get_position() const -> const glm::vec2& { return m_position; }

    auto set_local_position(const glm::vec2& pos) -> void;
    [[nodiscard]] auto get_local_position() const -> const glm::vec2& { return m_local_position; }

    auto set_rotation(float rot) -> void;
    [[nodiscard]] auto get_rotation() const -> float { return m_rotation; }

    auto set_euler_degrees(float deg) -> void;
    [[nodiscard]] auto get_euler_degrees() const -> float { return glm::degrees(m_rotation); }

    auto set_scale(const glm::vec2& s) -> void;
    [[nodiscard]] auto get_scale() const -> const glm::vec2& { return m_scale; }

private:
    auto update_child_position() -> void;
    auto update_child_rotation() -> void;
    auto update_child_scale() -> void;

    auto add_child(const std::shared_ptr<transform>& child) -> void;
    auto remove_child(const std::shared_ptr<transform>& child) -> void;

    std::weak_ptr<transform> m_parent;
    std::vector<std::shared_ptr<transform>> m_children = {};

    glm::vec2 m_position = { 0.0f, 0.0f };
    glm::vec2 m_local_position = { 0.0f, 0.0f };

    float m_rotation = 0.0f;
    float m_previous_parent_rotation = 0.0f;

    glm::vec2 m_scale = { 1.0f, 1.0f };
    glm::vec2 m_previous_parent_scale = { 1.0f, 1.0f };
};