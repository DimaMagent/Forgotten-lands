#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <atomic>
#include <cstdint>
#include <algorithm>
#include <utility>
#include "IRemovableDelegate.hpp"
#include "DelegateToken.hpp"

namespace sl {

	template<typename... Args>
	class LockFreeDelegate: public sl::IRemovableDelegate{
	public:
		LockFreeDelegate() {
			functions.store(std::make_shared<std::vector<Entry>>());
			nextId.store(1, std::memory_order_relaxed);
		}

		~LockFreeDelegate() {
			auto snapshot = functions.load(std::memory_order_acquire);
			if (snapshot && !snapshot->empty()) {
				for (const auto& e : *snapshot) {
					if (auto tokenPtr = e.token.lock()) {
						tokenPtr->delegateCleanup();
					}
				}
			}
		}

		[[nodiscard]] std::shared_ptr<DelegateToken> addFunction(std::function<void(Args...)> func) {
			const delegate_token id = nextId.fetch_add(1, std::memory_order_relaxed);
			std::shared_ptr<DelegateToken> token = std::make_shared<DelegateToken>(id, *this);
			auto fptr = std::make_shared<std::function<void(Args...)>>(std::move(func));

			auto expected = functions.load(std::memory_order_acquire);
			for (;;) {
				auto copy = std::make_shared<std::vector<Entry>>(*expected);
				copy->push_back(Entry{ token, fptr});
				if (functions.compare_exchange_weak(expected, copy,
				                                    std::memory_order_release,
				                                    std::memory_order_acquire)) {
					return token;
				}
			}
		}

		void removeFunction(delegate_token id) override{
			auto expected = functions.load(std::memory_order_acquire);
			for (;;) {
				bool found = false;
				for (const auto &e : *expected) {
					auto token = e.token.lock();
					if (!token) { continue; }
					if (token->getToken() == id) { found = true; break; }
				}
				if (!found) return;

				auto copy = std::make_shared<std::vector<Entry>>();
				copy->reserve(expected->size());
				for (const auto &e : *expected) {
					auto token = e.token.lock();
					if (!token) { continue; }
					if (token->getToken() != id) copy->push_back(e);
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
				(*e.func)(std::forward<Args>(args)...);
			}
		}

	private:
		struct Entry {
			std::weak_ptr<DelegateToken> token;
			std::shared_ptr<std::function<void(Args...)>> func;
		};

		mutable std::atomic<std::shared_ptr<std::vector<Entry>>> functions;
		std::atomic<delegate_token> nextId;
	};

}
