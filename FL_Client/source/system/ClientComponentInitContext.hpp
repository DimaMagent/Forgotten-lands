#pragma once

class TextureManager;
class IPlayerEntityStorage;
namespace sf {
	class RenderTarget;
}

struct ClientComponentInitContext {
	ClientComponentInitContext(TextureManager& textureManager, const IPlayerEntityStorage& playerEntityStorage) :
		textureManager(textureManager), playerEntityStorage(playerEntityStorage){}
	TextureManager& textureManager;
	const IPlayerEntityStorage& playerEntityStorage;
};