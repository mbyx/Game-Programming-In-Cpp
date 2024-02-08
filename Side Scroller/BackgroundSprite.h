#pragma once
#include "Sprite.h"

class BackgroundSprite : public Sprite {
public:
	// By default the drawOrder is lower, as it is a background.
	BackgroundSprite(Actor* owner, int drawOrder = -1) : Sprite(owner, drawOrder) {
		mScreenSize = Vec2D{1024, 768};
		mScrollSpeed = 100.0f;
	}

	~BackgroundSprite() {
		for (auto texture : mBackgroundTextures) {
			SDL_DestroyTexture(texture.mTexture);
		}
	}

	void Update(float delta) override {
		Sprite::Update(delta);

		for (auto& background : mBackgroundTextures) {
			background.mOffset.x += mScrollSpeed * delta;
			int width;
			SDL_QueryTexture(background.mTexture, nullptr, nullptr, &width, nullptr);
			if (abs(background.mOffset.x) > width * mOwner->GetScale()) {
				background.mOffset.x = (mBackgroundTextures.size() - 1) * width * mOwner->GetScale() - 1;
			}
		}
	}

	void Draw(SDL_Renderer* renderer) override {
		for (auto& background : mBackgroundTextures) {
			int width, height;
			SDL_QueryTexture(background.mTexture, nullptr, nullptr, &width, &height);
			
			SDL_Rect bgRect;
			bgRect.x = background.mOffset.x;
			bgRect.y = background.mOffset.y;
			bgRect.w = width * mOwner->GetScale();
			bgRect.h = height * mOwner->GetScale();
			
			SDL_RenderCopy(renderer, background.mTexture, nullptr, &bgRect);
		}
	}

	void SetBackgroundTextures(const std::vector<SDL_Texture*>& textures) {
		int count = 0;
		for (auto texture : textures) {
			BackgroundTexture t;

			int width;
			SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
			
			t.mTexture = texture;
			t.mOffset.x = count * width * mOwner->GetScale();
			t.mOffset.y = 0;

			mBackgroundTextures.emplace_back(t);

			count++;
		}
	}

	void SetScreenSize(const Vec2D& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() { return mScrollSpeed; }

private:
	struct BackgroundTexture {
		SDL_Texture* mTexture;
		Vec2D mOffset;
	};

	std::vector<BackgroundTexture> mBackgroundTextures;
	Vec2D mScreenSize;
	float mScrollSpeed;
};