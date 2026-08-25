#pragma once
#include <cstdint>
#include <vector>
#include <bit>
#include <stdexcept>
#include <concepts>
#include <type_traits>

namespace sl::net
{
	template<typename T>
	using uint_equivalent_t = 
		std::conditional_t<sizeof(T) == 1, uint8_t,
		std::conditional_t<sizeof(T) == 2, uint16_t,
		std::conditional_t<sizeof(T) == 4, uint32_t, uint64_t>>>;

	template<typename T>
	concept Serializable = std::is_trivially_copyable_v<T> && !std::is_pointer_v<T>;

	//platform-independent recording
	template<Serializable T>
	size_t write(std::vector<uint8_t>& out, T value) {
		using UIntType = uint_equivalent_t<T>;
		UIntType raw_bits = std::bit_cast<UIntType>(value);

		for (size_t i = 0; i < sizeof(T); ++i) {
			uint8_t byte = static_cast<uint8_t>((raw_bits >> ((sizeof(T) - 1 - i) * 8)) & 0xFF);
			out.push_back(byte);
		}
		return sizeof(T);
	}

	//platform-independent reading
	template<Serializable T>
	T read(const std::vector<uint8_t>& in, size_t& offset) {
		if (offset + sizeof(T) > in.size()) {
			throw std::out_of_range("Buffer overflow during read");
		}

		using UIntType = uint_equivalent_t<T>;
		UIntType raw_bits = 0;

		for (size_t i = 0; i < sizeof(T); ++i) {
			raw_bits = (raw_bits << 8) | in[offset + i];
		}

		offset += sizeof(T);
		return std::bit_cast<T>(raw_bits);
	}

	constexpr uint32_t fnv1a(const char* str) {
		uint32_t hash = 2166136261u;
		while (*str) {
			hash ^= (uint8_t)*str++;
			hash *= 16777619u;
		}
		return hash;
	}

}