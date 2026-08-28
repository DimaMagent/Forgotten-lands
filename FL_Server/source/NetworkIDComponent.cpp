#pragma once
#include "NetworkIDComponent.hpp"
#include "Entity.hpp"

NetworkIDComponent::NetworkIDComponent()
{
	token = 0;
}

void NetworkIDComponent::setSessionToken(sessionToken newToken)
{
	token = newToken;
}

void NetworkIDComponent::initialize(sl::ComponentInitContext context)
{
	context.entity.addComponent<NetworkIDComponent>();
}