#include <Enemy.h>
#include <Logger.h>
#include <ResourceManager.h>
#include <Sprite.h>
#include <Types.h>
#include <components/AnimationRenderer.h>
#include <components/PixelCollider.h>
#include <rendering/Shader.h>
#include <Actor.h>

namespace
{
	const std::vector<std::string> squid_frames = {
		"assets/Invaders/space__0000_A1.png",
		"assets/Invaders/space__0001_A2.png",
	};

	const std::vector<std::string> crab_frames = {
		"assets/Invaders/space__0002_B1.png",
		"assets/Invaders/space__0003_B2.png",
	};

	const std::vector<std::string> octopus_frames = {
		"assets/Invaders/space__0004_C1.png",
		"assets/Invaders/space__0005_C2.png",
	};

	const std::vector<std::string> ufo_frames = {
		"assets/Invaders/space__0007_UFO.png",
	};
}

Enemy::Enemy(World& world, const EnemyType& type) : Actor{ world }, m_enemy_type{ type }
{

}

void Enemy::build_enemy_frames_by_type()
{
	auto frames = std::vector<std::string>{};

	switch (m_enemy_type)
	{
	case Squid:   frames = squid_frames; break;
	case Crab:    frames = crab_frames; break;
	case Octopus: frames = octopus_frames; break;
	case Ufo:     frames = ufo_frames; break;
	case None:    return;
	default: break;  // NOLINT(clang-diagnostic-covered-switch-default)
	}

	if (frames.empty()) return;

	auto anim_clip = AnimationRenderer::AnimationClip{};

	for (auto&& path : frames)
	{
		const auto enemy_sprite_frame = std::make_shared<Sprite>(path);
		anim_clip.frames.push_back({ .frame_sprite = enemy_sprite_frame, .duration = 0.7f });
		add_component<PixelCollider>(enemy_sprite_frame);
	}

	add_component<AnimationRenderer>();
	const auto anim_rend = get_component<AnimationRenderer>();
	anim_rend->set_animation("idle", anim_clip);
	anim_rend->play("idle", true);
}

void Enemy::start()
{
	Actor::start();

	build_enemy_frames_by_type();
}

void Enemy::update(const float delta_time)
{
	Actor::update(delta_time);

}

void Enemy::on_collision_enter(const HitInfo& hit)
{
	CollisionListener::on_collision_enter(hit);

	LOG_INFO("Enemy on_collision_enter with {}", static_cast<int>(m_enemy_type));
}