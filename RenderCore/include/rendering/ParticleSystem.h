#pragma once

#include <Enums.h>
#include <memory>
#include <Sprite.h>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Particle
{
	glm::vec2 position = {};
	glm::vec2 scale = { 1.0f, 1.0f };
	glm::vec4 color = {};
	glm::vec2 velocity = {};
	float speed = {};
	float life_time = {};
	float rotation = {};
	bool is_active = {};
	bool should_expire = {};
};

class Shader;
class Texture;

auto direction_by_shape_type(ShapeType type) -> glm::vec2;

class ParticleSystem
{
	static constexpr float particle_quad_vertices[] = {
		0.0f, 1.0f,       0.0f, 1.0f,
		0.0f, 0.0f,       0.0f, 0.0f,
		1.0f, 0.0f,       1.0f, 0.0f,

		0.0f, 1.0f,       0.0f, 1.0f,
		1.0f, 0.0f,       1.0f, 0.0f,
		1.0f, 1.0f,       1.0f, 1.0f
	};

public:
	~ParticleSystem() = default;
	ParticleSystem() = delete;
	ParticleSystem(glm::vec2 position, unsigned amount, float min_speed, float max_speed, float min_life_time, float max_life_time);
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem(ParticleSystem&&) = delete;
	ParticleSystem& operator=(const ParticleSystem&) = delete;
	ParticleSystem& operator=(ParticleSystem&&) = delete;

	auto set_shape_type(const ShapeType shape_type) -> void { m_shape_type = shape_type; }
	auto set_on_finish_callback(const std::function<void()>& callback) -> void { m_on_finish_callback = callback; }
	auto reset() -> void;

	[[nodiscard]] const auto& emitting() const { return m_is_emitting; }
	auto& emitting() { return m_is_emitting; }

	[[nodiscard]] const auto& expire() const { return m_should_expire; }
	auto& expire() { return m_should_expire; }

	auto set_emission_time_interval(float interval) -> void;
	auto randomize_particles_position_between_bounds(float x_min, float x_max, float y_min, float y_max) -> void;
	auto set_position(const glm::vec2& position) -> void;
	auto update(float delta_time) -> void;
	auto render(const RenderContext& ctx) const -> void;

private:
	std::vector<Particle> m_particles = {};
	std::vector<Particle> m_active_particles = {};

	GLuint m_quad_vao = {};
	GLuint m_quad_vbo = {};
	GLuint m_instance_vbo = {};
	GLuint m_colors_vbo = {};

	glm::vec2 m_position = {};
	glm::vec2 m_start_pos = {};

	ShapeType m_shape_type = {};

	float m_min_speed = {};
	float m_max_speed = {};
	float m_min_life_time = {};
	float m_max_life_time = {};
	float m_emission_timer = {};
	float m_emission_interval = 0.004f;

	bool m_is_emitting = false;
	bool m_should_expire = false;

	unsigned m_amount = {};

	std::shared_ptr<Texture> m_texture = {};

	std::function<void()> m_on_finish_callback = {};

private:
	auto generate_buffers() -> void;
	auto initialize_particles() -> void;
	auto activate_or_create_particle() -> void;
	auto update_instance_buffer(const std::vector<Particle>& particles) const -> void;
};