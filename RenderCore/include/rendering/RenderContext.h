#pragma once

#include <glm/glm.hpp>
#include <memory>

struct RenderContext
{
	glm::mat4 view = {};
	glm::mat4 projection = {};
	std::shared_ptr<class Shader> shader = {};
	bool enable_debug = {};
};