#pragma once
#include <SFML/System/Vector2.hpp>
#include "system/systems/Component.hpp"
#include "network/Serializable.hpp"
#include "network/NetUtils.hpp"
#include "system/handlers/LockFreeDelegate.hpp"
#include "system/Cell.hpp"

namespace sl {

	class Entity;

	class TransformComponent: public sl::Component, public sl::Serializable {
	public:
		// sends previous position and new position
		sl::LockFreeDelegate<const sf::Vector2f&, const sf::Vector2f&> OnPositionChanged;

		sl::LockFreeDelegate<const sf::Vector2f&> OnSizeChanged;

		TransformComponent();
		TransformComponent(sf::Vector2f startPosition, sf::Vector2f size);
		TransformComponent(float x, float y, float sizeX, float sizeY);

		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);

		void setRotation(int x, int y);
		void setRotation(const sf::Vector2i& rotation);

		void setSize(float newSizeX, float newSizeY);
		void setSize(const sf::Vector2f& newSize);

		sf::Vector2f getPosition() const { return position; }
		sf::Vector2i getRotation() const { return rotation; }
		sf::Vector2f getSize() const { return size; }

		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(TransformComponent);
	private:
		sf::Vector2f position;

		// as normal
		sf::Vector2i rotation;

		sf::Vector2f size;

		REGISTER_COMPONENT();
	};
}