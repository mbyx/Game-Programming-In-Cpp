#pragma once

#include "Sprite.h"

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(Actor* owner, int drawOrder = 1) : Sprite(owner, drawOrder) {
		mAnimationFPS = 24;
		mCurrentFrame = 0;
	}

	~AnimatedSprite() {
		for (auto texture : mAnimationTextures) {
			SDL_DestroyTexture(texture);
		}
	}

	void Update(float delta) override {
		if (mAnimationTextures.size() > 0) {
			mCurrentFrame += mAnimationFPS * delta;
			while (mCurrentFrame >= mAnimationTextures.size()) {
				mCurrentFrame -= mAnimationTextures.size();
			}

			Sprite::SetTexture(mAnimationTextures[static_cast<int>(mCurrentFrame)]);
		}
	}

	void SetAnimationTextures(const std::vector<SDL_Texture*>& textures) {
		mAnimationTextures = textures;
		mCurrentFrame = 0;
		Sprite::SetTexture(mAnimationTextures[0]);
	}

	float GetAnimationFPS() const { return mAnimationFPS; }
	void SetAnimationFPS(float fps) { mAnimationFPS = fps; }

	float GetCurrentFrame() { return mCurrentFrame; }
	float SetCurrentFrame(float frame) { mCurrentFrame = frame; }

private:
	std::vector<SDL_Texture*> mAnimationTextures;
	float mCurrentFrame;
	float mAnimationFPS;
};