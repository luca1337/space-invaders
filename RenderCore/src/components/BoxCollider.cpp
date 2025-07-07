#include <Actor.h>
#include <Sprite.h>
#include <components/BoxCollider.h>
#include <components/Transform.h>

BoxCollider::BoxCollider(const std::shared_ptr<Actor>& owner) : Collider{ owner } 
{
}

auto BoxCollider::get_bounds() const -> glm::vec4
{
	const auto& ow = owner().lock();
	if (!ow) { return {}; }

	const auto& transform = ow->transform();
	const auto& pos = transform->get_position();
	const auto& size = transform->get_scale(); // oppure sprite->size() se non � uniforme

	return { pos.x, pos.y, size.x, size.y };
}

auto BoxCollider::collides_with(const Collider& other, glm::vec2& impact_point) const -> bool
{
	const auto& other_bounds = other.get_bounds();
	const auto& self_bounds = get_bounds();

	const auto ax1 = self_bounds.x;
	const auto ay1 = self_bounds.y;
	const auto ax2 = self_bounds.x + self_bounds.z;
	const auto ay2 = self_bounds.y + self_bounds.w;

	const auto bx1 = other_bounds.x;
	const auto by1 = other_bounds.y;
	const auto bx2 = other_bounds.x + other_bounds.z;
	const auto by2 = other_bounds.y + other_bounds.w;

	if (ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1)
	{
		impact_point = 
		{
			(ax1 + ax2) / 2.0f,
			(ay1 + ay2) / 2.0f
		};

		return true;
	}

	return false;
}
