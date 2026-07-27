#pragma once
#include <unordered_map>
#include <cstdint>
#include <functional>
#include "Serializable.hpp"
#include "Component.hpp"
#include "Entity.hpp"

class NetworkComponentRegistry {
public:
	using Instantiator = std::function<sl::Serializable* (sl::Entity&)>;

	NetworkComponentRegistry();

	template<typename T>
	void registerComponent() {
		static_assert(std::is_base_of_v<sl::Component, T>, "T must be a Component");
		static_assert(std::is_base_of_v<sl::Serializable, T>, "T must be Serializable to be network-instantiated");

		registry.try_emplace(T::TypeId = [](sl::Entity& entity) -> sl::Serializable* {
			T* comp = entity.getComponent<T>();
			if (!comp) {
				comp = &entity.addComponent<T>();
			}
			return static_cast<sl::Serializable*>(comp);
			});
	}

	sl::Serializable* createAndAttach(uint32_t typeId, sl::Entity& entity) const;
private:

	void registerComponents();
	
	std::unordered_map<uint32_t, Instantiator> registry;
};