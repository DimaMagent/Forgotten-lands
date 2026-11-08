#pragma once
#include <vector>


namespace sl {
	template <typename... Types>
	class TypeList {
	};

	template <typename... Types>
	std::vector<std::type_index> toTypeVector(TypeList<Types...>) {
		return {
			std::type_index(typeid(Types))...
		}
	}

	template<typename>
	struct is_type_list : std::false_type {};

	template<typename... Types>
	struct is_type_list<sl::TypeList<Types...>> : std::true_type {};

	template<typename T>
	inline constexpr bool is_type_list_v = is_type_list<T>::value;

}