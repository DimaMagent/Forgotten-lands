#pragma once
#include <vector>

namespace sl {

	struct SystemMetaData {

		std::vector<std::type_index> components;

		std::vector<std::type_index> requires;

		std::vector<std::type_index> produces;
		
	};

}