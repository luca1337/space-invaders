#pragma once

#include <Color.h>
#include <Export.h>
#include <string>
#include <vector>
#include <glad/glad.h>

class RENDER_API Texture
{
public:
	~Texture();
	Texture(const Texture&);
	explicit Texture(const std::string &file_path);
	explicit Texture(unsigned width, unsigned height, Color color);

	[[nodiscard]] auto get_id() const { return m_texture_id; }

	[[nodiscard]] auto get_width() const { return m_width; }
	[[nodiscard]] auto get_height() const { return m_height; }

	[[nodiscard]] auto get_pixels() const -> const std::vector<unsigned char>& { return m_pixel_data; }
	[[nodiscard]] auto get_pixel(const int x, const int y) const -> const Color&;

	auto set_pixel(int x, int y, const Color& c) -> void;
	auto update_region(int x, int y, int width, int height) const -> void;
	auto update() const -> void;

private:
	GLuint m_texture_id = {};
	unsigned m_width = {};
	unsigned m_height = {};

	std::vector<unsigned char> m_pixel_data; // RGBA buffer
};
