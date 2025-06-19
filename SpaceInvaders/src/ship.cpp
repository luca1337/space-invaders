#include <ship.h>

#include <enums.h>
#include <input.h>
#include <components/pixel_collider.h>
#include <resource_manager.h>
#include <sprite.h>
#include <components/sprite_renderer.h>
#include <components/transform.h>
#include <world.h>
#include <rendering/camera.h>
#include <rendering/shader.h>

#include <bullet.h>
#include <components/player_controller.h>
#include <components/shooter_component.h>

ship::ship(world& w) : actor{ w } {}

void ship::start()
{
	actor::start();

	const auto player_sprite = std::make_shared<sprite>("assets/Invaders/space__0006_Player.png");

	m_sprite_component = add_component<sprite_renderer>(player_sprite);

	add_component<pixel_collider>(player_sprite);
	add_component<player_controller>();
	add_component<shooter_component>();

	const auto& transform = get_transform();
	const auto& window = get_world().get_window();

	transform->set_local_position(glm::vec2{ window->props().width / 2.0f - m_sprite_component->sprite_raw()->size().x * 0.5f, window->props().height - m_sprite_component->sprite_raw()->size().y * 4.0f });
}

void ship::update(const float delta_time)
{
	actor::update(delta_time);

	const auto& cam = get_world().get_camera();
	const auto sprite_shader = resource_manager::get_from_cache<shader>({ .m_resource_type = resource_type::shader, .m_name = "SpriteShader" });

	m_sprite_component->render({
		.view = cam->get_view_matrix(),
		.projection = cam->get_projection_matrix(),
		.shader = sprite_shader.value()
		});
}

void ship::on_collision_stay(const hit_info& hit)
{
}

