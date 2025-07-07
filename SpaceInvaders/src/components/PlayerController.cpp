#include <Actor.h>
#include <Input.h>
#include <Sprite.h>
#include <World.h>
#include <components/PlayerController.h>
#include <components/SpriteRenderer.h>
#include <components/Transform.h>
#include <glm/vec2.hpp>

PlayerController::PlayerController(const std::shared_ptr<Actor>& owner) : Component{ owner } { }

void PlayerController::update(const float delta_time)
{
	Component::update(delta_time);

	const auto& ow = owner().lock();

	if (!ow) { return; }

	const auto& transform = ow->transform();
	const auto& sr = ow->get_component<SpriteRenderer>();
	const auto& window = ow->get_world().get_window();

	if (!sr || !transform) { return; }

	const auto& position = transform->get_local_position();
	const float sprite_width = sr->sprite_raw()->size().x;
	const float window_width = window->props().width;

	const bool can_move_right = position.x + sprite_width < window_width;
	const bool can_move_left = position.x > 0.0f;

	constexpr float move_speed = 150.0f;
	auto new_position = position;

	if (Input::is_key_pressed(*window, SDLK_D) && can_move_right)
	{
		new_position.x += delta_time * move_speed;
	}
	if (Input::is_key_pressed(*window, SDLK_A) && can_move_left)
	{
		new_position.x -= delta_time * move_speed;
	}

	transform->set_local_position(new_position);
}