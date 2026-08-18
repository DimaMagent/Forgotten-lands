#include "pch.h"
#include "WeaponComponent.hpp"

sl::WeaponComponent::WeaponComponent(float attackWidth, float attackHeight)
{
	attackDistance = sf::Vector2f(attackWidth, attackHeight);
}
