#pragma once

#include <glm/vec2.hpp>
#include <memory>

class Actor;

struct HitInfo
{
	glm::vec2 point = { };
	std::shared_ptr<Actor> other = {};
};
