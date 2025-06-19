#pragma once

#include <enums.h>
#include <logger.h>
#include <memory>
#include <optional>
#include <ranges>

class iresource;

template <std::derived_from<iresource> Resource>
struct resource_params
{
    resource_type m_resource_type = {};
    std::string m_name = {};
    std::shared_ptr<Resource> m_resource = {};
};

template <std::derived_from<iresource> Resource>
using resources = std::vector<resource_params<Resource>>;

class resource_manager
{
public:
    template <std::derived_from<iresource> Resource>
    static void register_resource(const resource_params<Resource>& params)
    {
        if (!find_private(params)) m_resources<Resource>.push_back(params);
        else LOG_CRITICAL("This Resource will not be loaded as it's already registered, please use GetFromCache() to retrieve it.");
    }

    template <std::derived_from<iresource> Resource>
    [[nodiscard]] static auto get_from_cache(const resource_params<Resource>& params) -> std::optional<decltype(std::declval<resource_params<Resource>>().m_resource)> { return find_private(params).value().m_resource; }

    template <std::derived_from<iresource> Resource>
    static auto get_all_from_cache() { return m_resources<Resource>; }

private:
    template <std::derived_from<iresource> Resource>
    inline static resources<Resource> m_resources = {};

    template <std::derived_from<iresource> Resource, typename ReturnType = typename std::optional<resource_params<Resource>>::value_type>
    static auto find_private(const resource_params<Resource>& params) -> std::optional<ReturnType>
    {
        auto it = std::ranges::find(m_resources<Resource>, params.m_name, &resource_params<Resource>::m_name);
        return it != m_resources<Resource>.end() ? *it : std::optional<ReturnType>{};
    }
};