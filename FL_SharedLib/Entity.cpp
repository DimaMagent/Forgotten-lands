#include "pch.h"
#include "Entity.hpp"
#include "TransformComponent.hpp"

sl::Entity::Entity(const TransformComponent& tc) :transformComponent(std::make_unique<TransformComponent>(tc)) {}

sl::Entity::~Entity() = default;
