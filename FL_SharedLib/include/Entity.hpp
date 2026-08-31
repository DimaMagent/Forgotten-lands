#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <cstdint>
#include "Component.hpp"
#include "EntityType.hpp"
#include "EntityId.hpp"
#include "ComponentTypes.hpp"

namespace sl {
    
    class Serializable;

    class Entity {
    public:
		Entity(sl::EntityType type);
        ~Entity();

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;

        Entity(Entity&&) noexcept = default;
        Entity& operator=(Entity&&) noexcept = default;

        EntityId getId() const { return id; }

        sl::EntityType getEntityType() const { return type; }

        void setId(EntityId newId) { id = newId; }

        template<typename T>
        T* getComponent() const{
            static_assert(std::is_base_of_v<sl::Component, T>, "T must inherit from Component.");
            static_assert(requires { T::TypeId; }, "PacketT must have static TypeId field");
            auto it = std::lower_bound(components.begin(), components.end(), T::TypeId,
                [](const auto& pair, const TypeID& tid) {
                    return pair.first < tid;
                });
            if (it == components.end() || it->first != T::TypeId) return nullptr;
            return static_cast<T*>(it->second.get());
        }

        sl::Component* getComponent(TypeID ComponentTypeId) const;

        template<typename T>
        bool hasComponent() const{
            static_assert(std::is_base_of_v<sl::Component, T>, "T must inherit from Component.");
            static_assert(requires { T::TypeId; }, "PacketT must have static TypeId field");
            auto it = std::lower_bound(components.begin(), components.end(), T::TypeId,
                [](const auto& pair, const TypeID& tid) {
                    return pair.first < tid;
                });
            if (it == components.end() || it->first != T::TypeId) return false;
            return true;
        }

        bool hasComponent(TypeID ComponentTypeId) const;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            static_assert(std::is_base_of_v<sl::Component, T>, "T must inherit from Component.");
            static_assert(requires { T::TypeId; }, "PacketT must have static TypeId field");
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *comp;
            
            auto it = std::lower_bound(components.begin(), components.end(), T::TypeId,
                [](const auto& pair, const TypeID& tid) {
                    return pair.first < tid;
                });
            components.emplace(it, T::TypeId, std::move(comp));
            return ref;
        }

        template<typename Fn>
        void forEachSerialization(Fn&& fn) const{
            static_assert(std::is_invocable_v<Fn, const sl::Serializable&> || std::is_invocable_v<Fn, sl::Serializable&>);
            for (auto& [tid, comp] : components) {
                if (auto* c = dynamic_cast<sl::Serializable*>(comp.get())) {
                    fn(*c);
                }
            }
        }

        template<typename Fn>
        void forCurrentSerialization(TypeID TypeId, Fn&& fn) const {
            static_assert(std::is_invocable_v<Fn, const sl::Serializable&> || std::is_invocable_v<Fn, sl::Serializable&>);
            auto it = std::lower_bound(components.begin(), components.end(), TypeId,
                [](const auto& pair, const TypeID& tid) {
                    return pair.first < tid;
                });
            if (it == components.end() || it->first != TypeId) { return; }

            if (auto* c = dynamic_cast<sl::Serializable*>(it->second.get())) {
                fn(*c);
            }

        }

    private:
        sl::EntityId id;
        sl::EntityType type;
        std::vector<std::pair<TypeID, std::unique_ptr<sl::Component>>> components;
    };
}