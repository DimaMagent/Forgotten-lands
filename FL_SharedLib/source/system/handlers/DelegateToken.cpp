#include "system/handlers/DelegateToken.hpp"


sl::DelegateToken::DelegateToken(sl::delegate_token token, IRemovableDelegate& removableDelegate)
{
	this->token = token;
	this->removableDelegate = &removableDelegate;
}

sl::DelegateToken::~DelegateToken()
{
	if (removableDelegate) {
		removableDelegate->removeFunction(token);
	}
}

void sl::DelegateToken::delegateCleanup()
{
	removableDelegate = nullptr;
}
