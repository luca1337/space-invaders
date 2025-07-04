#include <bullet.h>

#include <components/animation_renderer.h>
#include <hit_info.h>
#include <components/particle_emitter.h>
#include <components/pixel_collider.h>
#include <resource_manager.h>
#include <sprite.h>
#include <components/sprite_renderer.h>
#include <components/transform.h>
#include <world.h>
#include <rendering/camera.h>
#include <rendering/shader.h>

#include <shield.h>

// -- Player bullet

player_bullet::player_bullet(world& w) : actor{ w } {}

void player_bullet::start()
{
	actor::start();

	const auto bullet_sprite = std::make_shared<sprite>("assets/Projectiles/Projectile_Player.png");

	m_sprite_component = add_component<sprite_renderer>(bullet_sprite);

	add_component<pixel_collider>(bullet_sprite);

	/* Initialize particle system */

	auto cfg = particle_emitter::config{
		.amount = 500,
		.min_speed = 1.3f,
		.max_speed = 25.4f,
		.min_life_time = 0.1f,
		.max_life_time = 0.77f,
		.emission_interval = 0.004f,
		.shape = shape_type::circle,
		.expire = true,
	};

	m_impact_fx = add_component<particle_emitter>(cfg);

	m_impact_fx->get_emitter()->set_on_finish_callback([this]()
		{
			m_impact_fx->get_emitter()->emitting() = false;
			m_impact_fx->get_emitter()->reset();
		});

	m_impact_fx->start();

}

auto player_bullet::update(const float delta_time) -> void
{
	actor::update(delta_time);

	const auto& transform = get_transform();
	auto pos = transform->get_position();

	pos.y -= m_velocity.y * delta_time;

	transform->set_position(pos);

	m_impact_fx->update(delta_time);
}

void player_bullet::on_collision_enter(const hit_info& hit)
{
	collision_listener::on_collision_enter(hit);

	if (const auto& to_shield = std::dynamic_pointer_cast<shield>(hit.other))
	{
		to_shield->take_damage(hit.point);
	}

	m_impact_fx->get_emitter()->set_position(hit.point);
	m_impact_fx->get_emitter()->emitting() = true;
}  

// -- Enemy bullet

enemy_bullet::enemy_bullet(world& w) : actor{ w }
{
	auto bullet_anim = animation_renderer::animation_clip{};
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<sprite>("assets/Projectiles/ProjectileC_1.png"), .duration = 0.1f });
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<sprite>("assets/Projectiles/ProjectileC_2.png"), .duration = 0.1f });
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<sprite>("assets/Projectiles/ProjectileC_3.png"), .duration = 0.1f });
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<sprite>("assets/Projectiles/ProjectileC_4.png"), .duration = 0.1f });

	m_animation_component = std::make_shared<animation_renderer>(shared_from_this());
	m_animation_component->set_animation("bullet", bullet_anim);
	m_animation_component->play("bullet", true);
}

auto enemy_bullet::update(const float delta_time) -> void
{
	actor::update(delta_time);

	const auto& cam = get_world().get_camera();

	const auto sprite_shader = resource_manager::get_from_cache<shader>({ .m_resource_type = resource_type::shader, .m_name = "SpriteShader" });

	m_animation_component->render({
		.view = cam->get_view_matrix(),
		.projection = cam->get_projection_matrix(),
		.shader = sprite_shader.value()
		});
}

void enemy_bullet::on_collision_enter(const hit_info& hit)
{
	collision_listener::on_collision_enter(hit);
}
