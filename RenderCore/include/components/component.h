#pragma once

#include <memory>
#include <optional>
#include <rendering/RenderContext.h>

class Actor;

class Component : public std::enable_shared_from_this<Component>
{
public:
    virtual ~Component() = default;
	explicit Component(const std::weak_ptr<Actor>& owner) : m_owner(owner) {}

    virtual void start() {}
    virtual void update(float delta_time) {}
    virtual void on_destroy() {}

    // --- Owner ---
    [[nodiscard]] const auto& owner() const { return m_owner; }
    auto& owner() { return m_owner; }

    // --- Enable flag (getter/setter stile fluent) ---
    [[nodiscard]] const auto& enabled() const { return m_enabled; }
    auto& enabled()  { return m_enabled; }

    // --- Debug flag (getter/setter stile fluent) ---
    [[nodiscard]] const auto& debug() const { return m_debug; }
    auto& debug() { return m_debug; }

    // --- Render context ---
    [[nodiscard]] const auto& render_context() const { return m_render_context; }
    auto& render_context() { return m_render_context; }

protected:
    std::weak_ptr<Actor> m_owner = {};
    std::optional<RenderContext> m_render_context = {};
    bool m_enabled = true;
    bool m_debug = false;
};