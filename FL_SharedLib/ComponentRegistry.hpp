#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace sl {
    class Component;

    class ComponentRegistry {
    public:
        using Creator = std::function<std::unique_ptr<Component>()>;

		template<typename T>
        void registerComponent(const std::string& name) {
			creators.emplace(name, []() { return std::make_unique<T>(); });
        }

        std::unique_ptr<Component> create(const std::string& name) const;

    private:
        std::unordered_map<std::string, Creator> creators;
    };
}