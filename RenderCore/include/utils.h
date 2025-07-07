#pragma once

#include <Logger.h>

#include <vector>
#include <glad/glad.h>

namespace utils::gl
{
    [[nodiscard]] inline auto generate_open_gl_texture(const GLsizei width, const GLsizei height, const void *pixels, const GLint internal_format = GL_RGBA,
                                                    const GLenum format = GL_RGBA, const GLint wrap_s = GL_REPEAT, const GLint wrap_t = GL_REPEAT, const bool generate_mip_maps = false) -> GLuint
    {
        auto texture_id = GLuint{};

        glGenTextures(1, &texture_id);
        glActiveTexture(texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

        if (generate_mip_maps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texture_id;
    }

    inline auto generate_vertex_buffer(GLuint &vao, GLuint &vbo, const std::vector<float> &vertices) -> void
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(vertices[0])), vertices.data(), GL_STATIC_DRAW);
    }

    inline auto upload_vertex_attribute(const GLuint slot, const GLint slot_size, const GLsizei stride, const GLuint attribute_array_index, const void *ptr) -> void
    {
        glVertexAttribPointer(slot, slot_size, GL_FLOAT, GL_FALSE, stride, ptr);
        glEnableVertexAttribArray(attribute_array_index);
    }

    inline auto generate_frame_buffer_object(const GLuint width, const GLuint height, GLuint &fbo, GLuint &texture_color_buffer) -> void
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &texture_color_buffer);
        glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_CRITICAL("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
} // namespace utils::gl