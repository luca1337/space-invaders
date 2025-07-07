#include <Bullet.h>
#include <HitInfo.h>
#include <ResourceManager.h>
#include <Shield.h>
#include <Sprite.h>
#include <World.h>
#include <components/AnimationRenderer.h>
#include <components/ParticleEmitter.h>
#include <components/PixelCollider.h>
#include <components/SpriteRenderer.h>
#include <components/Transform.h>
#include <rendering/Camera.h>
#include <rendering/Shader.h>

// -- Player bullet

PlayerBullet::PlayerBullet(World& w) : Actor{ w } {}

void PlayerBullet::start()
{
	Actor::start();

	const auto bullet_sprite = std::make_shared<Sprite>("assets/Projectiles/Projectile_Player.png");

	m_sprite_component = add_component<SpriteRenderer>(bullet_sprite);

	add_component<PixelCollider>(bullet_sprite);

	/* Initialize particle system */

	auto cfg = ParticleEmitter::config{
		.amount = 500,
		.min_speed = 1.3f,
		.max_speed = 25.4f,
		.min_life_time = 0.1f,
		.max_life_time = 0.77f,
		.emission_interval = 0.004f,
		.shape = ShapeType::circle,
		.expire = true,
	};

	m_impact_fx = add_component<ParticleEmitter>(cfg);

	m_impact_fx->get_emitter()->set_on_finish_callback([this]()
		{
			m_impact_fx->get_emitter()->emitting() = false;
			m_impact_fx->get_emitter()->reset();
		});

	m_impact_fx->start();

}

auto PlayerBullet::update(const float delta_time) -> void
{
	Actor::update(delta_time);

	const auto& tf = transform();
	auto pos = tf->get_position();

	pos.y -= m_velocity.y * delta_time;

	tf->set_position(pos);

	m_impact_fx->update(delta_time);
}

void PlayerBullet::on_collision_enter(const HitInfo& hit)
{
	CollisionListener::on_collision_enter(hit);

	if (const auto& to_shield = std::dynamic_pointer_cast<Shield>(hit.other))
	{
		to_shield->take_damage(hit.point);
	}

	m_impact_fx->get_emitter()->set_position(hit.point);
	m_impact_fx->get_emitter()->emitting() = true;
}  

// -- Enemy bullet

EnemyBullet::EnemyBullet(World& w) : Actor{ w }
{
	auto bullet_anim = AnimationRenderer::AnimationClip{};
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<Sprite>("assets/Projectiles/ProjectileC_1.png"), .duration = 0.1f });
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<Sprite>("assets/Projectiles/ProjectileC_2.png"), .duration = 0.1f });
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<Sprite>("assets/Projectiles/ProjectileC_3.png"), .duration = 0.1f });
	bullet_anim.frames.push_back({ .frame_sprite = std::make_shared<Sprite>("assets/Projectiles/ProjectileC_4.png"), .duration = 0.1f });

	m_animation_component = std::make_shared<AnimationRenderer>(shared_from_this());
	m_animation_component->set_animation("bullet", bullet_anim);
	m_animation_component->play("bullet", true);
}

auto EnemyBullet::update(const float delta_time) -> void
{
	Actor::update(delta_time);

	const auto& cam = get_world().get_camera();

	const auto sprite_shader = ResourceManager::get_from_cache<Shader>({ .m_resource_type = ResourceType::shader, .m_name = "SpriteShader" });

	m_animation_component->render({
		.view = cam->get_view_matrix(),
		.projection = cam->get_projection_matrix(),
		.shader = sprite_shader.value()
		});
}

void EnemyBullet::on_collision_enter(const HitInfo& hit)
{
	CollisionListener::on_collision_enter(hit);
}
