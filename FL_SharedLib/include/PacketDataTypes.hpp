#pragma once
#include <cstdint>
namespace sl::net {
	enum InputState : uint8_t {
		IS_None = 0,
		IS_MoveUp = 1 << 0,
		IS_MoveDown = 1 << 1,
		IS_MoveLeft = 1 << 2,
		IS_MoveRight = 1 << 3,

		Mask_Vertical = IS_MoveUp | IS_MoveDown,
		Mask_Horizontal = IS_MoveLeft | IS_MoveRight
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