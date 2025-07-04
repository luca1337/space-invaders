#pragma once

struct render_context;

class irenderable
{
public:
	virtual auto render(const render_context& ctx) -> void = 0;
	virtual ~irenderable() = default;
};
