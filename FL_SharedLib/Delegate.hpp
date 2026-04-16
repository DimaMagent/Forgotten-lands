#pragma once
#include <vector>
#include <functional>
#include <mutex>


namespace sl {

	/*This delegate is designed for small broadcast volumes and frequent addFunction.
	If you need a delegate for frequent broadcasts, it's best to consider the lock-free option.*/
	template<typename... Args>
	class Delegate {
	public:
		Delegate() = default;
		void addFunction(std::function<void(Args...)> func) {
			std::lock_guard<std::mutex> lock(mtx);
			functions.push_back(func);
		}
		void broadcast(Args... args) {
			std::lock_guard<std::mutex> lock(mtx);
			for (const auto& func : functions) {
				func(args...);
			}
		}

	private:
		std::vector<std::function<void(Args...)>> functions;
		mutable std::mutex mtx;

	};
}