#pragma once

#include <color.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>

class transform;
class shader;
class texture;
struct render_context;

class sprite
{
public:
	~sprite();
	sprite();
	sprite(const sprite& other);
	explicit sprite(const std::string& texture_path);
	explicit sprite(unsigned int width, unsigned int height, const color& c);

	void render(const render_context& ctx, const transform& tf) const;

	[[nodiscard]] const auto& sprite_color() const { return m_color; }
	auto& sprite_color() { return m_color; }

	[[nodiscard]] const auto& size() const { return m_size; }
	auto& size() { return m_size; }

	[[nodiscard]] const auto& sprite_texture() const { return m_texture; }
	auto& sprite_texture() { return m_texture; }

private:
	unsigned int m_vao = {};
	unsigned int m_vbo = {};

	std::shared_ptr<texture> m_texture = {};
	color m_color = {};
	glm::vec2 m_size = {};
};
