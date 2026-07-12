#pragma once
#include <memory>
#include <vector>
#include "Delegate.hpp"

namespace sf {
	class RenderTarget;
}
namespace sl {
	class Entity;
}

class RenderManager {
public:
	RenderManager(sf::RenderTarget& renderTarget);

	void render(sl::Entity& entity) const;
private:
	sf::RenderTarget& renderTarget;
};