#pragma once
#include "Vec2D.h"
#include "SDL.h"

struct Paddle {
	Vec2D mPos;

	int mDir;
	int mWidth;
	int mHeight;

public:
	Paddle(float x, float y) {
		mPos = Vec2D{ x, y };
		mDir = 0;
		mWidth = 15;
		mHeight = 100;
	}

	void Update(float delta) {
		if (mDir != 0) {
			mPos.y += mDir * 300.0f * delta;

			if (mPos.y < (mHeight / 2) + mWidth) {
				mPos.y = (float) ((mHeight / 2) + mWidth);
			} else if (mPos.y > (768 - ((mHeight / 2) + mWidth))) {
				mPos.y = (float) (768 - ((mHeight / 2) + mWidth));
			}
		}
	}

	void Draw(SDL_Renderer* mRenderer) {
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		
		SDL_Rect paddle{
			mPos.x - mWidth,
			static_cast<int>(mPos.y - mHeight / 2),
			mWidth,
			mHeight
		};
		SDL_RenderFillRect(mRenderer, &paddle);
	}
};