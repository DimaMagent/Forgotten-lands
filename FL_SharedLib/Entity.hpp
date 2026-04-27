#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Component.hpp"

namespace sl {

    class Entity {
    public:
		Entity() = default;
        ~Entity();

        Entity(const Entity&) = delete;             // запрет копирования
        Entity& operator=(const Entity&) = delete;

        Entity(Entity&&) = default;                 // разрешить перемещение
        Entity& operator=(Entity&&) = default;

        template<typename T>
        T* getComponent() {
            auto it = components.find(typeid(T));
            if (it == components.end()) return nullptr;
            return static_cast<T*>(it->second.get());
        }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *comp;
            components.try_emplace(typeid(T), std::move(comp));
            return ref;
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<sl::Component>> components;
    };
}