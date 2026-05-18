#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <cstdint>
#include "Component.hpp"

namespace sl {
    
    class Serializable;

    class Entity {
    public:
		Entity();
        ~Entity();

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;

        Entity(Entity&&) = default;
        Entity& operator=(Entity&&) = default;

        uint32_t getId() const { return id; }

        uint32_t getGlobalId() const { return globalId; }

        void setGlobalId(uint32_t newId) { globalId = newId; }

        template<typename T>
        T* getComponent() {
            static_assert(std::is_base_of_v<sl::Component, T>, "T must inherit from Component.");
            static_assert(requires { T::TypeId; }, "PacketT must have static TypeId field");
            auto it = std::lower_bound(components.begin(), components.end(), T::TypeId,
                [](const auto& pair, const uint32_t& tid) {
                    return pair.first < tid;
                });
            if (it == components.end() || it->first != T::TypeId) return nullptr;
            return static_cast<T*>(it->second.get());
        }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            static_assert(std::is_base_of_v<sl::Component, T>, "T must inherit from Component.");
            static_assert(requires { T::TypeId; }, "PacketT must have static TypeId field");
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *comp;
            
            auto it = std::lower_bound(components.begin(), components.end(), T::TypeId,
                [](const auto& pair, const uint32_t& tid) {
                    return pair.first < tid;
                });
            components.emplace(it, T::TypeId, std::move(comp));
            return ref;
        }

        template<typename Fn>
        void forEachSerialization(Fn&& fn) {
            static_assert(std::is_invocable_v<Fn, const sl::Serializable&> || std::is_invocable_v<Fn, sl::Serializable&>);
            for (auto& [tid, comp] : components) {
                if (auto* c = dynamic_cast<sl::Serializable*>(comp.get())) {
                    fn(*c);
                }
            }
        }

        template<typename Fn>
        void forCurrentSerialization(uint32_t TypeId, Fn&& fn) {
            static_assert(std::is_invocable_v<Fn, const sl::Serializable&> || std::is_invocable_v<Fn, sl::Serializable&>);
            auto it = std::lower_bound(components.begin(), components.end(), TypeId,
                [](const auto& pair, const uint32_t& tid) {
                    return pair.first < tid;
                });
            if (it == components.end() || it->first != TypeId) { return; }

            if (auto* c = dynamic_cast<sl::Serializable*>(it->second.get())) {
                fn(*c);
            }

        }

    private:
        static uint32_t nextId;
        uint32_t id;
        uint32_t globalId = 0;
        std::vector<std::pair<uint32_t, std::unique_ptr<sl::Component>>> components;
    };
}