#pragma once
#include <memory>
#include "SimulationEntity.hpp"

class RenderComponent;
namespace sf {
	class RenderTarget;
}

class VisualEntity: public sl::SimulationEntity {
public:
	VisualEntity() = default;
	VisualEntity(const RenderComponent& rc, const sl::TransformComponent& tc);
	virtual ~VisualEntity();
	void render(sf::RenderTarget& target) const;
protected:
	std::shared_ptr<RenderComponent> renderComponent;
};