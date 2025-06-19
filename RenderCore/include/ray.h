#pragma once

#include <glm/vec3.hpp>

class ray
{
public:
    ~ray() = default;
    ray() = default;
    ray(const glm::vec3& origin, const glm::vec3& direction) : m_origin{origin}, m_direction{direction} {}
    ray(const ray&) = default;
    ray(ray&&) = default;
    ray& operator=(const ray&) = default;
    ray& operator=(ray&&) = default;

    [[nodiscard]] auto origin() const -> glm::vec3 { return m_origin;}
    [[nodiscard]] auto direction() const -> glm::vec3 { return m_direction; }
    [[nodiscard]] auto at(const float t) const -> glm::vec3 { return m_origin + t * m_direction; }

private:
    glm::vec3 m_origin = {};
    glm::vec3 m_direction = {};
};