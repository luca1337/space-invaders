#include <enums.h>
#include <logger.h>
#include <random.h>
#include <sprite.h>
#include <texture.h>
#include <utils.h>
#include <rendering/camera.h>
#include <rendering/particle_system.h>
#include <rendering/shader.h>

auto direction_by_shape_type(const shape_type type) -> glm::vec2
{
	auto direction = glm::vec2{};

	switch (type)
	{
	case shape_type::half_circle:
	{
		const auto random_angle = rng::generate_random_number<float>(-glm::pi<float>(), 2 * glm::pi<float>());
		direction = { glm::cos(random_angle), glm::sin(random_angle) };
	}
	break;
	case shape_type::circle:
	{
		const auto random_angle = rng::generate_random_number<float>(0.0f, 2 * glm::pi<float>());
		direction = { glm::cos(random_angle), glm::sin(random_angle) };
	}
	break;
	case shape_type::none:
		direction = { rng::generate_random_number(-1.0f, 1.0f), rng::generate_random_number(-1.0f, 1.0f) };
		break;
	default:
		break;
	}

	return direction;
}

particle_system::particle_system(const glm::vec2 position, const unsigned amount, const float min_speed, const float max_speed, const float min_life_time, const float max_life_time)
	: m_position{ position }, m_start_pos{ position }, m_shape_type{ shape_type::circle },
	m_min_speed{ min_speed }, m_max_speed{ max_speed }, m_min_life_time{ min_life_time }, m_max_life_time{ max_life_time }, m_amount{ amount }
{
	m_particles.resize(amount);
	m_active_particles.resize(amount);
	generate_buffers();
}

auto particle_system::reset() -> void
{
	m_emission_timer = 0.f;
	m_active_particles.clear();

	for (auto& [position, scale, color, velocity, speed, life_time, rotation, is_active, should_expire] : m_particles)
	{
		position = m_start_pos;
		velocity = direction_by_shape_type(m_shape_type);
		speed = rng::generate_random_number(m_min_speed, m_max_speed);
		life_time = rng::generate_random_number(m_min_life_time, m_max_life_time);
		color = glm::vec4{
			rng::generate_random_number(0.3f, 1.0f),
			rng::generate_random_number(0.3f, 1.0f),
			rng::generate_random_number(0.3f, 1.0f),
			1.0f
		};
		is_active = false;
		should_expire = false;
	}

	update_instance_buffer(m_particles);
}

auto particle_system::set_emission_time_interval(const float interval) -> void
{
	constexpr auto min_emission_interval = 0.004f;
	m_emission_interval = glm::max(min_emission_interval, interval);
}

auto particle_system::randomize_particles_position_between_bounds(const float x_min, const float x_max, const float y_min, const float y_max) -> void
{
	for (auto& p : m_particles)
	{
		const auto new_x = m_start_pos.x + rng::generate_random_number(x_min, x_max);
		const auto new_y = m_start_pos.y + rng::generate_random_number(y_min, y_max);
		p.position = glm::vec2{ new_x, new_y };
	}

	update_instance_buffer(m_particles);
}

auto particle_system::set_position(const glm::vec2& position) -> void
{
	m_start_pos = position;

	for (auto& p : m_particles)
	{
		p.position = position;
	}

	update_instance_buffer(m_particles);
}

auto particle_system::update(float delta_time) -> void
{
	if (!m_is_emitting) { return; }

	m_emission_timer += delta_time;

	for (unsigned i = 0ul; i != m_amount; ++i)
	{
		activate_or_create_particle();
	}

	std::erase_if(m_active_particles, [delta_time](auto& p)
		{
			if (p.is_active)
			{
				p.life_time -= delta_time;

				if (p.life_time > 0.0f)
				{
					p.position += p.velocity * delta_time * p.speed;
					return false;
				}
			}
			return true;
		});

	update_instance_buffer(m_active_particles);

	// Notify on finish callback if all particles are expired and not emitting
	if (m_active_particles.empty() && m_on_finish_callback)
	{
		m_on_finish_callback();
	}
}

auto particle_system::render(const render_context& ctx) const -> void
{
	if (!m_is_emitting) { return; }

	ctx.shader->bind();

	if (m_texture)
	{
		const auto texture_id = m_texture->get_id();
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	const auto view = ctx.view;
	const auto projection = ctx.projection;

	ctx.shader->set_matrix_4x4("view", view);
	ctx.shader->set_matrix_4x4("projection", projection);

	glBindVertexArray(m_quad_vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_active_particles.size());
	glBindVertexArray(0);

	ctx.shader->unbind();
}

auto particle_system::generate_buffers() -> void
{
	m_texture = std::make_shared<texture>(30, 30, color{ .r = 255, .g = 0, .b = 0, .a = 30 });

	glGenVertexArrays(1, &m_quad_vao);
	glGenBuffers(1, &m_quad_vbo);
	glGenBuffers(1, &m_instance_vbo);
	glGenBuffers(1, &m_colors_vbo);

	glBindVertexArray(m_quad_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad_vertices), particle_quad_vertices, GL_STATIC_DRAW);

	utils::gl::upload_vertex_attribute(0, 2, 4 * sizeof(float), 0, nullptr);
	utils::gl::upload_vertex_attribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

	initialize_particles();
	update_instance_buffer(m_particles);

	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
	for (auto i = 0ul; i != 4; ++i)
	{
		constexpr auto vec4_size = sizeof(glm::vec4);
		glEnableVertexAttribArray(2 + i);
		glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(i * vec4_size));
		glVertexAttribDivisor(2 + i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_colors_vbo);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

auto particle_system::initialize_particles() -> void
{
	for (auto& [position, scale, color, velocity, speed, life_time, rotation, is_active, should_expire] : m_particles)
	{
		position = m_start_pos;
		velocity = direction_by_shape_type(m_shape_type);
		speed = rng::generate_random_number(m_min_speed, m_max_speed);
		life_time = rng::generate_random_number(m_min_life_time, m_max_life_time);
		color = glm::vec4{
			rng::generate_random_number(0.3f, 1.0f),
			rng::generate_random_number(0.3f, 1.0f),
			rng::generate_random_number(0.3f, 1.0f),
			1.0f
		};
	}
}

auto particle_system::activate_or_create_particle() -> void
{
	for (auto& p : m_particles)
	{
		if (!p.is_active && !p.should_expire)
		{
			p.is_active = true;
			m_active_particles.push_back(p);
			return;
		}
	}

	if (m_should_expire) { return; }

	auto p = particle{};
	p.is_active = true;
	p.life_time = rng::generate_random_number(m_min_life_time, m_max_life_time);
	p.position = m_start_pos;
	p.velocity = direction_by_shape_type(m_shape_type);
	p.speed = rng::generate_random_number(m_min_speed, m_max_speed);

	m_particles.push_back(p);

	LOG_INFO("Created new particle, current count: {}", m_particles.size());
}

template <typename T, typename F>
auto transform_particles(const std::vector<particle>& particles,const F& transform_func)
{
	std::vector<T> results(particles.size());
	std::transform(particles.begin(), particles.end(), results.begin(), transform_func);
	return results;
}

auto particle_system::update_instance_buffer(const std::vector<particle>& particles) const -> void
{
	const auto model_matrices = transform_particles<glm::mat4>(particles, [](const auto& p)
		{
			const auto translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(p.position, 0.0f));
			const auto rotation = glm::rotate(glm::identity<glm::mat4>(), p.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			const auto scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3{ p.scale, 1.0f });
			return translation * scale;
		});

	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(glm::mat4), model_matrices.data(), GL_DYNAMIC_DRAW);

	const auto colors = transform_particles<glm::vec4>(particles, [](const auto& p) {
		return p.color;
		});

	glBindBuffer(GL_ARRAY_BUFFER, m_colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), colors.data(), GL_DYNAMIC_DRAW);
}