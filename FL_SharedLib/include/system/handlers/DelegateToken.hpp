#pragma once
#include <memory>
#include "IRemovableDelegate.hpp"

namespace sl {
	class DelegateToken {
	public:
		DelegateToken(sl::delegate_token token, IRemovableDelegate& removableDelegate);
		~DelegateToken();

		void delegateCleanup();
		sl::delegate_token getToken() const { return token; }
	private:
		sl::delegate_token token;
		IRemovableDelegate* removableDelegate;
	};
}