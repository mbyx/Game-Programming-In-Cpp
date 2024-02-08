#include "AssetManager.h"

#include <algorithm>
#include <string>

AssetManager::AssetManager(const char* basePath = "Assets/") {
	mBasePath = basePath;
}

AssetManager::~AssetManager() {
	while (!mSprites.empty()) {
		delete mSprites.back();
	}
}

SDL_Texture* AssetManager::LoadTexture(const char* fileName, SDL_Renderer* renderer) {
	std::string fullPath(mBasePath);
	fullPath.append(fileName);

	SDL_Surface* surface = IMG_Load(fullPath.c_str());
	if (!surface) {
		SDL_Log("Failed to load texture file: %s", SDL_GetError());
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) {
		SDL_Log("Failed to convert surface to a texture: %s", SDL_GetError());
		return nullptr;
	}

	return texture;
}

void AssetManager::Draw(SDL_Renderer* renderer) {
	for (auto sprite : mSprites) {
		sprite->Draw(renderer);
	}
}

void AssetManager::AddSprite(Sprite* sprite) {
	mSprites.emplace_back(sprite);
	
	// Larger draw order goes first.
	std::sort(mSprites.begin(), mSprites.end(), [](Sprite* const a, Sprite* const b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void AssetManager::RemoveSprite(Sprite* sprite) {
	auto position = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (position != mSprites.end()) {
		mSprites.erase(position);
	}
}