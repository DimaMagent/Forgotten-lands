#include "pch.hpp"
#include "ClientSystemUpdater.hpp"
#include <memory>


ClientSystemUpdater::ClientSystemUpdater(const ClientSystemCreatorContext& context): SystemUpdater(context) {
	creatorContext = std::make_unique<ClientSystemCreatorContext>(context);
}

