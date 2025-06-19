#pragma once

#include <iresource.h>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string_view>

class RENDER_API shader final : public iresource
{
public:
    shader() = default;
    shader(std::string_view vertex, std::string_view fragment);

    auto bind() const { glUseProgram(m_program_id); }
    auto unbind() const { glUseProgram(0); }

    auto set_matrix_4x4(const std::string_view name, const glm::mat4& m) { glUniformMatrix4fv(glGetUniformLocation(m_program_id, name.data()), 1, false, &m[0][0]); }
    auto set_float(const std::string_view name, const float value) { glUniform1f(glGetUniformLocation(m_program_id, name.data()), value); }
    auto set_vec3(const std::string_view name, const glm::vec3& value) { glUniform3fv(glGetUniformLocation(m_program_id, name.data()), 1, &value[0]); }
    auto set_vec2(const std::string_view name, const glm::vec2& value) { glUniform2fv(glGetUniformLocation(m_program_id, name.data()), 1, &value[0]); }
    auto set_int(const std::string_view name, const int value) { glUniform1i(glGetUniformLocation(m_program_id, name.data()), value); }
    auto set_uint(const std::string_view name, const uint32_t value) { glUniform1ui(glGetUniformLocation(m_program_id, name.data()), value); }
    auto set_bool(const std::string_view name, const bool value) { glUniform1i(glGetUniformLocation(m_program_id, name.data()), value); }

    [[nodiscard]] auto get_id() const { return m_program_id; }

private:
    GLuint m_program_id = {};
};