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

	template<typename T>
	concept Time = requires {
		typename T::rep;
		typename T::period;
	}&& std::is_same_v<
		std::remove_cvref_t<T>,
		std::chrono::duration<typename T::rep, typename T::period>
	>;

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

		template<Time Duration>
		static void addDefferedCall(std::function<void()>&& func, Duration time) {
			if (!io) return;

			auto steady_d = std::chrono::duration_cast<asio::steady_timer::duration>(time);
			auto timer = std::make_shared<asio::steady_timer>(*io, steady_d);

			timer->async_wait([timer, callback = std::move(func)](const asio::error_code& ec) {
				if (!ec && callback) {
					callback();
				}
				});
		}

		template<SafeTimerArgs... Args, CallableWith<Args...> Func, Time Duration>
		static void addDefferedCall(Func&& func, Duration time, Args... args) {
			if (!io) return;

			auto boundTask = [cb = std::forward<Func>(func), ...boundArgs = args]() mutable {
				cb(boundArgs...);
				};

			auto steady_d = std::chrono::duration_cast<asio::steady_timer::duration>(time);
			addDefferedCall(std::function<void()>(std::move(boundTask)), steady_d);
		}

		template<SafeTimerArgs... Args, CallableWith<Args...> Func, Time Duration>
		static void addLoopedDefferedCall(Func&& func,
			Duration firstTimeCall,
			Duration delay,
			bool isEndless,
			uint8_t loopCount,
			Args... args)
		{
			if (!io) return;

			auto boundTask = [cb = std::forward<Func>(func), ...boundArgs = args]() mutable {
				cb(boundArgs...);
				};

			auto steady_first = std::chrono::duration_cast<asio::steady_timer::duration>(firstTimeCall);
			auto steady_delay = std::chrono::duration_cast<asio::steady_timer::duration>(delay);
			auto timer = std::make_shared<asio::steady_timer>(*io, steady_first);
			auto self = std::make_shared<std::function<void(const asio::error_code&)>>();
			auto counter = std::make_shared<uint8_t>(0);

			*self = [timer, task = std::function<void()>(std::move(boundTask)), steady_delay, isEndless, loopCount, counter, self](const asio::error_code& ec) mutable {
				if (ec) return;

				task();

				if (isEndless || (++(*counter) < loopCount)) {
					timer->expires_at(timer->expiry() + steady_delay);
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