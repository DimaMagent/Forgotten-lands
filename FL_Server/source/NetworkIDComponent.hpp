#pragma once
#include "Component.hpp"
#include <cstdint>

using sessionToken = uint32_t;

class NetworkIDComponent: public sl::Component {
public:
	NetworkIDComponent();

	void setSessionToken(sessionToken newToken);

	sessionToken getSessionToken() const { return token; }

	COMPONENT_TYPE(NetworkIDComponent);

private:
	sessionToken token;

	REGISTER_COMPONENT();
};