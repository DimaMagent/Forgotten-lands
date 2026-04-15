#include "Character.hpp"
#include "RenderComponent.hpp"

Character::Character(RenderComponent& rc) : Entity(std::make_shared<RenderComponent>(rc))
{
}

Character::~Character() = default;

