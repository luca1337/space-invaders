#pragma once

#include <Color.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>

class Transform;
class Shader;
class Texture;
struct RenderContext;

class Sprite
{
public:
	~Sprite();
	Sprite();
	Sprite(const Sprite& other);
	explicit Sprite(const std::string& texture_path);
	explicit Sprite(unsigned int width, unsigned int height, const Color& c);

	void render(const RenderContext& ctx, const Transform& tf) const;

	[[nodiscard]] const auto& sprite_color() const { return m_color; }
	auto& sprite_color() { return m_color; }

	[[nodiscard]] const auto& size() const { return m_size; }
	auto& size() { return m_size; }

	[[nodiscard]] const auto& sprite_texture() const { return m_texture; }
	auto& sprite_texture() { return m_texture; }

private:
	unsigned int m_vao = {};
	unsigned int m_vbo = {};

	std::shared_ptr<Texture> m_texture = {};
	Color m_color = {};
	glm::vec2 m_size = {};
};
