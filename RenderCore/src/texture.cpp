#include <Logger.h>
#include <Texture.h>
#include <Utils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture()
{
    if (m_texture_id != 0)
    {
        glDeleteTextures(1, &m_texture_id);
        m_texture_id = 0;
    }
}

Texture::Texture(const Texture& other)
    : m_width(other.m_width), m_height(other.m_height), m_pixel_data(other.m_pixel_data)
{
    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixel_data.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const std::string& file_path)
{
    int width, height, nr_channels;
    if (const auto data = stbi_load(file_path.c_str(), &width, &height, &nr_channels, 4))
    {
        m_width = width;
        m_height = height;
        m_pixel_data.assign(data, data + width * height * 4);

        m_texture_id = utils::gl::generate_open_gl_texture(static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height), data, GL_RGBA, GL_RGBA, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        stbi_image_free(data);
    }
    else
    {
        LOG_CRITICAL("Failed to load texture");
    }
}

Texture::Texture(const unsigned width, const unsigned height, const Color color) : m_width(width), m_height(height), m_pixel_data(width * height * 4)
{
    for (unsigned i = 0; i < width * height; ++i)
    {
        m_pixel_data[static_cast<std::vector<unsigned char, std::allocator<unsigned char>>::size_type>(i) * 4 + 0] = color.r;
        m_pixel_data[static_cast<std::vector<unsigned char, std::allocator<unsigned char>>::size_type>(i) * 4 + 1] = color.g;
        m_pixel_data[static_cast<std::vector<unsigned char, std::allocator<unsigned char>>::size_type>(i) * 4 + 2] = color.b;
        m_pixel_data[static_cast<std::vector<unsigned char, std::allocator<unsigned char>>::size_type>(i) * 4 + 3] = color.a;
    }

    m_texture_id = utils::gl::generate_open_gl_texture(static_cast<GLsizei>(width), static_cast<GLsizei>(height), m_pixel_data.data(), GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, true);
}

auto Texture::get_pixel(const int x, const int y) const -> const Color&
{
	if (x < 0 || y < 0 || x >= static_cast<int>(m_width) || y >= static_cast<int>(m_height))
	{
        return black;
	}

	const int flipped_y = m_height - y - 1;
	const size_t index = (flipped_y * m_width + x) * 4;
	return { m_pixel_data[index + 0], m_pixel_data[index + 1], m_pixel_data[index + 2], m_pixel_data[index + 3] };
}

auto Texture::set_pixel(const int x, const int y, const Color& c) -> void
{
    if (x < 0 || y < 0 || x >= static_cast<int>(m_width) || y >= static_cast<int>(m_height))
    {
        return;
    }

    const int flipped_y = m_height - y - 1;
    const size_t index = (flipped_y * m_width + x) * 4;

    m_pixel_data[index + 0] = c.r;
    m_pixel_data[index + 1] = c.g;
    m_pixel_data[index + 2] = c.b;
    m_pixel_data[index + 3] = c.a;
}

auto Texture::update_region(const int x, const int y, const int width, const int height) const -> void
{
    if (m_texture_id == 0 || m_pixel_data.empty()) return;
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &m_pixel_data[(y * m_width + x) * 4]);
}

auto Texture::update() const -> void
{
    if (m_texture_id == 0 || m_pixel_data.empty()) return;
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height), GL_RGBA, GL_UNSIGNED_BYTE, m_pixel_data.data());
}