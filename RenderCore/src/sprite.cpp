#include <sprite.h>
#include <texture.h>
#include <utils.h>
#include <components/transform.h>
#include <glad/glad.h>
#include <rendering/render_context.h>
#include <rendering/shader.h>

const auto quad_vertices = std::vector{
    0.0f, 1.0f,       0.0f, 1.0f,
    0.0f, 0.0f,       0.0f, 0.0f,
    1.0f, 0.0f,       1.0f, 0.0f,

    0.0f, 1.0f,       0.0f, 1.0f,
    1.0f, 0.0f,       1.0f, 0.0f,
    1.0f, 1.0f,       1.0f, 1.0f
};

sprite::~sprite()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

sprite::sprite()
{
    utils::gl::generate_vertex_buffer(m_vao, m_vbo, quad_vertices);
    utils::gl::upload_vertex_attribute(0, 2, 4 * sizeof(float), 0, nullptr);
    utils::gl::upload_vertex_attribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));
}

sprite::sprite(const sprite& other)
{
    utils::gl::generate_vertex_buffer(m_vao, m_vbo, quad_vertices);
    utils::gl::upload_vertex_attribute(0, 2, 4 * sizeof(float), 0, nullptr);
    utils::gl::upload_vertex_attribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    m_texture = other.m_texture;
    m_size = other.m_size;
}

sprite::sprite(const std::string& texture_path)
{
	utils::gl::generate_vertex_buffer(m_vao, m_vbo, quad_vertices);
	utils::gl::upload_vertex_attribute(0, 2, 4 * sizeof(float), 0, nullptr);
	utils::gl::upload_vertex_attribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

	m_texture = std::make_shared<texture>(texture_path);
	m_size = { static_cast<float>(m_texture->get_width()), static_cast<float>(m_texture->get_height()) };
}

sprite::sprite(const unsigned int width, const unsigned int height, const color& c)
{
    utils::gl::generate_vertex_buffer(m_vao, m_vbo, quad_vertices);
    utils::gl::upload_vertex_attribute(0, 2, 4 * sizeof(float), 0, nullptr);
    utils::gl::upload_vertex_attribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    m_texture = std::make_shared<texture>(width, height, c);
    m_size = { static_cast<float>(m_texture->get_width()), static_cast<float>(m_texture->get_height()) };
}

auto sprite::render(const render_context& ctx, const transform& tf) const -> void
{
    auto model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(tf.get_position(), 0.0f));
    model = glm::rotate(model, tf.get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(m_size * tf.get_scale(), 1.0f));

    ctx.shader->bind();

    if (m_texture)
    {
        glBindTexture(GL_TEXTURE_2D, m_texture->get_id());
    }

    ctx.shader->set_matrix_4x4("model", model);
    ctx.shader->set_matrix_4x4("view", ctx.view);
    ctx.shader->set_matrix_4x4("projection", ctx.projection);
    ctx.shader->set_float("alpha", 1.0f);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glPolygonMode(GL_FRONT_AND_BACK, ctx.enable_debug ? GL_LINE : GL_FILL);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (ctx.enable_debug)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    ctx.shader->unbind();
}