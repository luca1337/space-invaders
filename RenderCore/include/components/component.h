#pragma once

#include <memory>

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

protected:
    std::weak_ptr<actor> m_owner = {};
};