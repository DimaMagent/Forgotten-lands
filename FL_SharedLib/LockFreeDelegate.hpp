#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <atomic>
#include <cstdint>
#include <algorithm>

namespace sl {

	template<typename... Args>
	class LockFreeDelegate {
	public:
		LockFreeDelegate() {
			functions.store(std::make_shared<std::vector<Entry>>());
			nextId.store(1, std::memory_order_relaxed);
		}

		std::uint64_t addFunction(std::function<void(Args...)> func) {
			const std::uint64_t id = nextId.fetch_add(1, std::memory_order_relaxed);
			auto fptr = std::make_shared<std::function<void(Args...)>>(std::move(func));

			auto expected = functions.load(std::memory_order_acquire);
			for (;;) {
				auto copy = std::make_shared<std::vector<Entry>>(*expected);
				copy->push_back(Entry{id, fptr});
				if (functions.compare_exchange_weak(expected, copy,
				                                    std::memory_order_release,
				                                    std::memory_order_acquire)) {
					return id;
				}
			}
		}

		void removeFunction(std::uint64_t id) {
			auto expected = functions.load(std::memory_order_acquire);
			for (;;) {
				bool found = false;
				for (const auto &e : *expected) { if (e.id == id) { found = true; break; } }
				if (!found) return;

				auto copy = std::make_shared<std::vector<Entry>>();
				copy->reserve(expected->size());
				for (const auto &e : *expected) {
					if (e.id != id) copy->push_back(e);
				}
				if (functions.compare_exchange_weak(expected, copy,
				                                    std::memory_order_release,
				                                    std::memory_order_acquire)) {
					return;
				}
			}
		}

		void broadcast(Args... args) const {
			auto snapshot = functions.load(std::memory_order_acquire);
			if (!snapshot || snapshot->empty()) return;
			for (const auto& e : *snapshot) {
				(*e.func)(args...);
			}
		}

	private:
		struct Entry {
			std::uint64_t id;
			std::shared_ptr<std::function<void(Args...)>> func;
		};

		mutable std::atomic<std::shared_ptr<std::vector<Entry>>> functions;
		std::atomic<std::uint64_t> nextId;
	};

}
