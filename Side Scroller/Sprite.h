#pragma once

#include "Component.h"
#include "SDL.h"
#include "Actor.h"

class Sprite : public Component {
public:
	Sprite(Actor* owner, int drawOrder = 1) : Component(owner) {
		mDrawOrder = drawOrder;
		owner->AddComponent(this);
	}

	~Sprite() {
		SDL_DestroyTexture(mTexture);
	}

	virtual void Draw(SDL_Renderer* renderer) {
		if (mTexture) {
			SDL_Rect r;

			r.w = static_cast<int>(mTextureWidth * mOwner->GetScale());
			r.h = static_cast<int>(mTextureHeight * mOwner->GetScale());
			
			r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
			r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);
			
			SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -mOwner->GetRotation() * 180.0f / M_PI, nullptr, SDL_FLIP_NONE);
		}
	}

	virtual void SetTexture(SDL_Texture* texture) {
		mTexture = texture;

		SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
	}

	void SetDrawOrder(int drawOrder) {
		mDrawOrder = drawOrder;
	}

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTextureHeight() const { return mTextureHeight; }
	int GetTextureWidth() const { return mTextureWidth; }

protected:
	SDL_Texture* mTexture = nullptr;
	int mDrawOrder;


	int mTextureHeight = 0;
	int mTextureWidth = 0;
};