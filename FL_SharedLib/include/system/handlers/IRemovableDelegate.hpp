#pragma once
#include <cstdint>

namespace sl {
	using delegate_token = std::uint64_t;

	class IRemovableDelegate {
	public:
		virtual ~IRemovableDelegate() = default;
		virtual void removeFunction(delegate_token id) = 0;
	};

}