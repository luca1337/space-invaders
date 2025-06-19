#pragma once

#include <glm/vec2.hpp>
#include <memory>

class actor;

struct hit_info
{
	glm::vec2 point = { };
	std::shared_ptr<actor> other = {};
};
