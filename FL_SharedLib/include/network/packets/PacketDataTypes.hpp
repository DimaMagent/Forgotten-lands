#pragma once
#include <cstdint>


namespace sl::net {

	enum Action : uint8_t {
		None = 0,
		Attack = 1,
	};

	enum PacketType : uint8_t {
		PT_Auth = 0,
		PT_InputState = 1,
		PT_Input = 2,
		PT_Status = 3,
		PT_GameStateUpdate = 4,
	};

	struct ComponentData {
		uint32_t typeId = 0;
		uint32_t compSize = 0;
		std::vector<uint8_t> componentData;
	};

	struct EntityData {
		uint32_t entityId = 0;
		uint32_t entityType = 0;
		uint32_t entityDataSize = 0;
		std::vector<ComponentData> componentsData;

		static std::vector<EntityData> getEntityData(const std::vector<uint8_t>& data);
	};
}