#pragma once
#include <memory>

class RenderComponent;
class TransformComponent;
namespace sf {
	class RenderTarget;
}

class Entity {
public:
	Entity() = default;
	Entity(const RenderComponent& rc, const TransformComponent& tc);
	virtual ~Entity();
	void render(sf::RenderTarget& target) const;
protected:
	std::shared_ptr<RenderComponent> renderComponent;
	std::unique_ptr<TransformComponent> transformComponent;
};