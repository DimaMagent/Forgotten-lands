#pragma once
#include <memory>

class RenderComponent;
namespace sf {
	class RenderTarget;
}

class Entity {
public:
	Entity(std::shared_ptr<RenderComponent> rc);
	~Entity();
	void render(sf::RenderTarget& target) const;
private:
	std::shared_ptr<RenderComponent> renderComponent;
};