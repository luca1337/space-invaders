#pragma once

#include <window.h>

#include <functional>
#include <memory>

#include <rendering/shader.h>

class RENDER_API post_processing
{
public:
    virtual ~post_processing() = default;
    explicit post_processing(const window& window);

    virtual auto render_to_scene(const float delta_time, const std::function<void()>& render_scene_hook, const std::function<void()>& render_gui_hook = nullptr) -> void
    {
        // step 1: renderizzo la scena di gioco nel framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        // renderizzo la scena di gioco (senza GUI)
        render_scene_hook();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // step 2: applico il post-processing alla scena di gioco ( se esiste ) altrimenti renderizzo sulla texture piana ed in seguito applico degli effetti.
        #pragma region SHADER_LOGIC

		if (m_pp_shader)
		{
			m_pp_shader->bind();
		}
        
        #pragma endregion

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // step 3: renderizzo la GUI sopra la scena post-processata per evitare che venga preso in considerazione
        if (render_gui_hook) { render_gui_hook(); }
    }

protected:
    const window& m_Window;
    GLuint m_vao, m_vbo, m_fbo, m_texture_color_buffer;
    std::shared_ptr<shader> m_pp_shader = {};
};