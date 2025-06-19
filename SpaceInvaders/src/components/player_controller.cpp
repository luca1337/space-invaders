#include <components/player_controller.h>

#include <actor.h>
#include <input.h>
#include <sprite.h>
#include <components/sprite_renderer.h>
#include <components/transform.h>
#include <glm/vec2.hpp>

#include <game.h>
#include <world.h>

player_controller::player_controller(const std::shared_ptr<actor>& owner) : component{ owner } { }

void player_controller::update(const float delta_time)
{
	component::update(delta_time);

	const auto& owner = get_owner();
	const auto& transform = owner->get_transform();
	const auto& sr = owner->get_component<sprite_renderer>();
	const auto& window = get_owner()->get_world().get_window();

	if (!sr || !transform) { return; }

	const glm::vec2& position = transform->get_local_position();
	const float sprite_width = sr->sprite_raw()->size().x;
	const float window_width = window->props().width;

	const bool can_move_right = position.x + sprite_width < window_width;
	const bool can_move_left = position.x > 0.0f;

	constexpr float move_speed = 150.0f;
	auto new_position = position;

	if (input::is_key_pressed(*window, SDLK_D) && can_move_right)
	{
		new_position.x += delta_time * move_speed;
	}
	if (input::is_key_pressed(*window, SDLK_A) && can_move_left)
	{
		new_position.x -= delta_time * move_speed;
	}

	transform->set_local_position(new_position);
}