#pragma once

#include <Enums.h>
#include <Logger.h>
#include <memory>
#include <optional>
#include <ranges>

class IResource;

template <std::derived_from<IResource> Resource>
struct ResourceParams
{
    ResourceType m_resource_type = {};
    std::string m_name = {};
    std::shared_ptr<Resource> m_resource = {};
};

template <std::derived_from<IResource> Resource>
using Resources = std::vector<ResourceParams<Resource>>;

class ResourceManager
{
public:
    template <std::derived_from<IResource> Resource>
    static void register_resource(const ResourceParams<Resource>& params)
    {
        if (!find_private(params)) m_resources<Resource>.push_back(params);
        else LOG_CRITICAL("This Resource will not be loaded as it's already registered, please use GetFromCache() to retrieve it.");
    }

    template <std::derived_from<IResource> Resource>
    [[nodiscard]] static auto get_from_cache(const ResourceParams<Resource>& params) -> std::optional<decltype(std::declval<ResourceParams<Resource>>().m_resource)> { return find_private(params).value().m_resource; }

    template <std::derived_from<IResource> Resource>
    static auto get_all_from_cache() { return m_resources<Resource>; }

private:
    template <std::derived_from<IResource> Resource>
    inline static Resources<Resource> m_resources = {};

    template <std::derived_from<IResource> Resource, typename ReturnType = typename std::optional<ResourceParams<Resource>>::value_type>
    static auto find_private(const ResourceParams<Resource>& params) -> std::optional<ReturnType>
    {
        auto it = std::ranges::find(m_resources<Resource>, params.m_name, &ResourceParams<Resource>::m_name);
        return it != m_resources<Resource>.end() ? *it : std::optional<ReturnType>{};
    }
};