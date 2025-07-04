#pragma once

#include <memory>
#include <optional>
#include <rendering/render_context.h>

class actor;

class component : public std::enable_shared_from_this<component>
{
public:
    virtual ~component() = default;
	explicit component(const std::weak_ptr<actor>& owner) : m_owner(owner) {}

    virtual void start() {}
    virtual void update(float delta_time) {}
    virtual void on_destroy() {}

    [[nodiscard]] const auto& get_owner() const { return m_owner; }
    auto& get_owner() { return m_owner; }

    // --- Render context ---
    auto set_render_context(render_context ctx) { m_render_context = std::move(ctx); }
    [[nodiscard]] const auto& get_render_context() const { return m_render_context; }

protected:
    std::optional<render_context> m_render_context = {};

protected:
    std::weak_ptr<actor> m_owner = {};
};