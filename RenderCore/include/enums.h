#pragma once

#include <Export.h>

enum class RENDER_API ResourceType
{
	shader,
	texture,
	audio,
	render_ctx
};

enum class RENDER_API DirectionType
{
	left,
	right,
	down
};

enum class RENDER_API ShapeType
{
	none,
	circle,
	half_circle,
};