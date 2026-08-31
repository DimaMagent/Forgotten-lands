#pragma once
#include <cstdint>
#include <functional>

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

        bool operator==(const EntityId& other) const noexcept {
            return ID == other.ID;
        }

        bool operator<(const EntityId& other) const noexcept {
            return ID < other.ID;
        }

        bool operator>(const EntityId& other) const noexcept {
            return ID > other.ID;
        }
    };
}

namespace std {
    template <>
    struct hash<sl::EntityId> {
        std::size_t operator()(const sl::EntityId& e) const noexcept {
            std::size_t h1 = std::hash<uint32_t>{}(e.ID);
            return h1;
        }
    };
}