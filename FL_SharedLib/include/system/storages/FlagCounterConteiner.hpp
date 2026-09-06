#pragma once
#include <cstdint>
#include <type_traits>
#include <array>

namespace sl {

    template<typename EnumType, size_t EnumSize>
    class FlagCounterContainer {
    private:
        std::array<uint8_t, EnumSize> counts = { 0 };

    public:
        void AddFlag(EnumType flag) {
            auto val = static_cast<std::underlying_type_t<EnumType>>(flag);
            for (size_t i = 0; i < EnumSize; ++i) {
                if (val & (1 << i)) {
                    counts[i]++;
                }
            }
        }

        void RemoveFlag(EnumType flag) {
            auto val = static_cast<std::underlying_type_t<EnumType>>(flag);
            for (size_t i = 0; i < EnumSize; ++i) {
                if ((val & (1 << i)) && counts[i] > 0) {
                    counts[i]--;
                }
            }
        }

        // returns a simple bit mask for quick checking `if (GetMask() & StunState::Disarmed)`
        EnumType GetMask() const {
            uint8_t mask = 0;
            for (size_t i = 0; i < EnumSize; ++i) {
                if (counts[i] > 0) {
                    mask |= (1 << i);
                }
            }
            return static_cast<EnumType>(mask);
        }
    };

}