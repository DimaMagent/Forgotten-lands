#pragma once
#include <chrono>
#include <functional>
#include <asio\io_context.hpp>
#include <asio\steady_timer.hpp>
#include <cstdint>

namespace sl {

	template<typename T>
	concept PODOrPrimitive =
		!std::is_pointer_v<T> &&
		!std::is_reference_v<T> &&
		(std::is_fundamental_v<T> || std::is_enum_v<T>);

	template<typename... Args>
	concept SafeTimerArgs = (PODOrPrimitive<Args> && ...);

	template<typename F, typename... Args>
	concept CallableWith = std::invocable<F, Args...>;

	struct LoopOptions {
		LoopOptions(bool isEndless, uint8_t loopCount);

		bool isEndless = true;
		uint8_t loopCount = 2;
	};

	class DefferedFunctionStorage {
	public:
		static void init(asio::io_context& io_ctx) {
			io = &io_ctx;
		}

		static void addDefferedCall(std::function<void()>&& func, std::chrono::milliseconds time);

		template<SafeTimerArgs... Args, CallableWith<Args...> Func>
		static void addDefferedCall(Func&& func, std::chrono::milliseconds time, Args... args) {
			if (!io) return;

			auto boundTask = [cb = std::forward<Func>(func), ...boundArgs = args]() mutable {
				cb(boundArgs...);
				};

			addDefferedCall(std::function<void()>(std::move(boundTask)), time);
		}

		template<SafeTimerArgs... Args, CallableWith<Args...> Func>
		static void addLoopedDefferedCall(Func&& func,
			std::chrono::milliseconds firstTimeCall,
			std::chrono::milliseconds delay,
			bool isEndless,
			uint8_t loopCount,
			Args... args)
		{
			if (!io) return;

			auto boundTask = [cb = std::forward<Func>(func), ...boundArgs = args]() mutable {
				cb(boundArgs...);
				};

			auto timer = std::make_shared<asio::steady_timer>(*io, firstTimeCall);
			auto self = std::make_shared<std::function<void(const asio::error_code&)>>();
			auto counter = std::make_shared<uint8_t>(0);

			*self = [timer, task = std::function<void()>(std::move(boundTask)), delay, isEndless, loopCount, counter, self](const asio::error_code& ec) mutable {
				if (ec) return;

				task();

				if (isEndless || (++(*counter) < loopCount)) {
					timer->expires_at(timer->expiry() + delay);
					timer->async_wait(*self);
				}
				};

			timer->async_wait(*self);
		}

	private:
		DefferedFunctionStorage() = delete;

		inline static asio::io_context* io = nullptr;
	};
}