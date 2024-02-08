#pragma once

#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Sprite.h"

class AssetManager
{
public:
	AssetManager(const char* basePath);
	~AssetManager();
	
	SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* renderer);
	
	void Draw(SDL_Renderer* renderer);

	void AddSprite(Sprite* sprite);
	void RemoveSprite(Sprite* sprite);

private:
	const char* mBasePath;
	std::vector<Sprite*> mSprites;
};

