#pragma once

#include <export.h>

enum class RENDER_API resource_type
{
	shader,
	texture,
	audio
};

enum class RENDER_API direction_type
{
	left,
	right,
	down
};

enum class RENDER_API shape_type
{
	none,
	circle,
	half_circle,
};