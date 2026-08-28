#pragma once
#include <vector>
#include <cstdint>
#include <optional>
#include <concepts>

namespace sl {

    struct EntityId {
        uint16_t index : 24;
        uint16_t generation : 8;
    };

    template <typename T>
    concept ValidEntityData = (std::move_constructible<T> || std::copy_constructible<T>) &&
        std::destructible<T>;


    // SlotMap is a wrapper around std::vector for identifying stored entityData.
    template <ValidEntityData EntityData>
    class SlotMap {
        static_assert(std::is_nothrow_move_constructible_v<EntityData> || !std::is_move_constructible_v<EntityData>,
            "Warning: the type has a move constructor but lacks `noexcept`. The vector will copy data during reallocation!");
    private:
        struct Slot {
            uint32_t generation = 1;
            bool is_alive = false;

            // Union to avoid wasting extra memory
            // If is_alive == true -> data is used.
            //  If is_alive == false -> next_free is used.
            union {
                EntityData data;
                uint32_t next_free;
            };

            Slot() : next_free(0) {}
            ~Slot() {
                if (is_alive) { data.~EntityData(); }
            }
        };

        std::vector<Slot> slots;
        uint32_t free_list_head = UINT32_MAX;

    public:
        EntityId spawn(EntityData&& new_data) {
            if (free_list_head != UINT32_MAX) {

                uint32_t index = free_list_head;
                Slot& slot = slots[index];

                free_list_head = slot.next_free;

                slot.is_alive = true;
                new (&slot.data) EntityData(std::move(new_data));

                return EntityId{ index, slot.generation };
            }
            else {
                uint32_t index = static_cast<uint32_t>(slots.size());

                Slot slot;
                slot.is_alive = true;
                slot.generation = 1;
                new (&slot.data) EntityData(std::move(new_data));

                slots.push_back(std::move(slot));
                return EntityId{ index, 1 };
            }
        }

        void destroy(EntityId id) {
            if (id.index >= slots.size()) return;

            Slot& slot = slots[id.index];

            // A check to see whether it was an entity and whether it had already been destroyed.
            // Also, protection against double deletion or the use of an outdated ID.
            if (!slot.is_alive || slot.generation != id.generation) {
                return;
            }

            slot.data.~EntityData();
            slot.is_alive = false;

            slot.generation++;

            slot.next_free = free_list_head;
            free_list_head = id.index;
        }

        EntityData* get(EntityId id) {
            if (id.index < slots.size()) {
                Slot& slot = slots[id.index];
                if (slot.is_alive && slot.generation == id.generation) {
                    return &slot.data;
                }
            }
            return nullptr;
        }
    };
}