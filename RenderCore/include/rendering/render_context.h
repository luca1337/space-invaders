#pragma once

#include <glm/glm.hpp>
#include <memory>

struct render_context
{
	glm::mat4 view = {};
	glm::mat4 projection = {};
	std::shared_ptr<class shader> shader = {};
	bool enable_debug = {};
};