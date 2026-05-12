#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Component.hpp"

namespace sl {
    
    class Serializable;

    class Entity {
    public:
		Entity() = default;
        ~Entity();

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;

        Entity(Entity&&) = default;
        Entity& operator=(Entity&&) = default;

        template<typename T>
        T* getComponent() {
            auto it = std::lower_bound(components.begin(), components.end(), typeid(T),
                [](const auto& pair, const std::type_index& tid) {
                    return pair.first < tid;
                });
            if (it == components.end() || it->first != typeid(T)) return nullptr;
            return static_cast<T*>(it->second.get());
        }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *comp;
            auto it = std::lower_bound(components.begin(), components.end(), typeid(T),
                [](const auto& pair, const std::type_index& tid) {
                    return pair.first < tid;
                });
            components.emplace(it, typeid(T), std::move(comp));
            return ref;
        }

        template<typename Fn>
        void forEachSerialization(Fn&& fn) {
            static_assert(std::is_invocable_v<Fn, const Serializable&>);
            for (auto& [tid, comp] : components) {
                if (auto* c = dynamic_cast<sl::Serializable*>(comp.get())) {
                    fn(*c);
                }
            }
        }

    private:
        std::vector<std::pair<std::type_index, std::unique_ptr<sl::Component>>> components;
    };
}