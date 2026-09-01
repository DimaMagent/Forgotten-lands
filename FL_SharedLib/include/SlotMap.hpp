#pragma once
#include <vector>
#include <cstdint>
#include <optional>
#include <concepts>
#include "EntityId.hpp"

namespace sl {

    template <typename T>
    concept ValidEntityData = (std::move_constructible<T> || std::copy_constructible<T>) &&
        std::destructible<T>;

    // SlotMap is a wrapper around std::vector for identifying stored entityData.
    template <ValidEntityData EntityData>
    class SlotMap {
        static_assert(std::is_nothrow_move_constructible_v<EntityData> || !std::is_move_constructible_v<EntityData>,
            "Warning: the type has a move constructor but lacks `noexcept`. The vector will copy data during reallocation!");
    private:

        struct SparseSlot {
            sl::generationType generation = 1;
            bool is_alive = false;

            // if is_alive == true: denseIndex. Else NextFree
            uint32_t denseIndexOrNextFree = UINT32_MAX;

            SparseSlot(){}

        };
        std::vector<SparseSlot> sparse;
        std::vector<EntityData> dense;
        std::vector<uint32_t> sparse_dense_index;
        uint32_t free_list_head = UINT32_MAX;

    public:

        SlotMap(size_t capacity = 1000) {
            sparse.reserve(capacity);
            dense.reserve(capacity);
            sparse_dense_index.reserve(capacity);
        }

        SlotMap(const SlotMap&) = delete;
        SlotMap& operator=(const SlotMap&) = delete;

        SlotMap(SlotMap&&) noexcept = default;
        SlotMap& operator=(SlotMap&&) noexcept = default;

        ~SlotMap() = default;

        std::span<EntityData> view() {
           return dense;
        }

        std::span<const EntityData> view() const{
           return dense;
        }

        void reserve(size_t new_capacity) {
            sparse.reserve(new_capacity);
            dense.reserve(new_capacity);
            sparse_dense_index.reserve(new_capacity);
        }

        size_t capacity() const { return dense.capacity(); }
        size_t size() const { return dense.size(); }

        EntityId spawn(EntityData&& new_data) {
            if (free_list_head != UINT32_MAX) {

                uint32_t index = free_list_head;
                SparseSlot& slot = sparse[index];

                free_list_head = slot.denseIndexOrNextFree;

                slot.is_alive = true;

                dense.push_back(std::move(new_data));
                sparse_dense_index.emplace_back(index);
                
                slot.denseIndexOrNextFree = dense.size() - 1;

                return EntityId{ index, slot.generation };
            }
            else {
                uint32_t index = static_cast<uint32_t>(sparse.size());

                SparseSlot& slot = sparse.emplace_back();
                slot.is_alive = true;
                slot.generation = 1;

                dense.emplace_back(std::move(new_data));
                sparse_dense_index.emplace_back(index);

                slot.denseIndexOrNextFree = dense.size() - 1;

                return EntityId{ index, 1 };
            }
        }

        void destroy(EntityId id) {
            if (id.getIndex() >= sparse.size()) return;

            SparseSlot& slot = sparse[id.getIndex()];

            if (!slot.is_alive || slot.generation != id.getGeneration()) {
                return;
            }

            uint32_t removed_dense_idx = slot.denseIndexOrNextFree;
            uint32_t last_dense_idx = dense.size() - 1;

            //if (removed_dense_idx > dense.size()) { return; }

            if (removed_dense_idx != last_dense_idx) {
                dense[removed_dense_idx] = std::move(dense[last_dense_idx]);
                sparse_dense_index[removed_dense_idx] = std::move(sparse_dense_index[last_dense_idx]);

                uint32_t moved_sparse_idx = sparse_dense_index[removed_dense_idx];

                sparse[moved_sparse_idx].denseIndexOrNextFree = removed_dense_idx;
            }

            dense.pop_back();
            sparse_dense_index.pop_back();

            slot.is_alive = false;

            slot.generation++;

            slot.denseIndexOrNextFree = free_list_head;
            free_list_head = id.getIndex();
        }

        EntityData* get(EntityId id) {
            if (id.getIndex() < sparse.size()) {
                SparseSlot& slot = sparse[id.getIndex()];
                if (slot.is_alive && slot.generation == id.getGeneration()) {
                    return &dense[slot.denseIndexOrNextFree];
                }
            }
            return nullptr;
        }

        const EntityData* get(EntityId id) const{
            if (id.getIndex() < sparse.size()) {
                const SparseSlot& slot = sparse[id.getIndex()];
                if (slot.is_alive && slot.generation == id.getGeneration()) {
                    return &dense[slot.denseIndexOrNextFree];
                }
            }
            return nullptr;
        }
    };
}