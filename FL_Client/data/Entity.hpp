#pragma once
#include <memory>

class RenderComponent;
class TransformComponent;
namespace sf {
	class RenderTarget;
}

class Entity {
public:
	Entity(const RenderComponent& rc, const TransformComponent& tc);
	~Entity();
	void render(sf::RenderTarget& target) const;
protected:
	std::shared_ptr<RenderComponent> renderComponent;
	std::unique_ptr<TransformComponent> transformComponent;
};