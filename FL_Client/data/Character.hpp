#pragma once
#include <memory>
#include "Entity.hpp"


class Character : public Entity {
public:
	Character(RenderComponent& rc);
	~Character();
private:
};