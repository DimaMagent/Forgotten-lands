#pragma once
#include <memory>
#include "SystemUpdater.hpp"

namespace sl {
	class Entity;

	class UpdatableSystem {
	protected:
		#define RegisterUpdatableSystem() \
		static void registerSystem(){ \
			try{ \
				sl::SystemUpdater::registerSystem(std::function(initialize)); \
			} \
			catch(std::exception& e){ \
				std::cerr << " [CRITICAL] system registration error: " << e.what() << "\n"; \
				std::abort(); \
			} \
		} \
		inline static const bool isRegistered = (registerSystem(), true); \
		\
		static systemPtr initialize(sl::SystemCreatorContext context); \

	public:

		virtual ~UpdatableSystem() = default;

		virtual void update(float updateTime, const sl::Entity& entity) = 0;
	};

}