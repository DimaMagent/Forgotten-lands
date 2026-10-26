#pragma once
#include "system/systems/SystemUpdater.hpp"
#include "ClientSystemCreatorContext.hpp"


class ClientSystemUpdater : public sl::SystemUpdater {
public:
	ClientSystemUpdater(const ClientSystemCreatorContext& context);
};