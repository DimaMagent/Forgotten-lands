#pragma once
#include <cstdint>

namespace sl {
    struct EntityId {
        uint32_t ID;

        EntityId(uint32_t ID): ID(ID) {}

        EntityId() : ID(0){}

        EntityId(uint32_t idx, uint32_t generation) {
            ID = (generation << 24) | (idx & 0x00FFFFFF);
        }

        uint32_t getIndex() const {
            return ID & 0x00FFFFFF;
        }

        uint32_t getGeneration() const{
            return ID << 24;
        }

        bool operator==(EntityId& other) {
            return ID == other.ID;
        }

        bool operator!=(EntityId& other) {
            return ID != other.ID;
        }
    };
}