#include <cmath>
#include "Paddle.h"
#include "Vec2D.h"
#include "SDL.h"

const int maxVelocityMag = 500;

struct Ball {
	Vec2D mPos;
	Vec2D mVel;

	int mSize = 15;

public:
	Ball(float x, float y) {
		mPos = Vec2D{ x, y };
		mVel = randomVelocity();
	}

	void Update(float delta, Paddle paddleLeft, Paddle paddleRight) {
		mPos.x += mVel.x * delta;
		mPos.y += mVel.y * delta;
		
		bounceFromWalls(1024, 768);

		int yDiff = (int) abs(mPos.y - paddleLeft.mPos.y);
		if (yDiff <= (paddleLeft.mHeight / 2) && mPos.x <= (2 * paddleLeft.mWidth) && mPos.x >= paddleLeft.mWidth && mVel.x < 0.0f) {
			mVel.x *= -1.0f;
		}

		yDiff = (int)abs(mPos.y - paddleRight.mPos.y);
		if (yDiff <= (paddleRight.mHeight / 2) && mPos.x >= (1024 - 2 * paddleRight.mWidth) && mPos.x <= (1024 - paddleRight.mWidth) && mVel.x > 0.0f) {
			mVel.x *= -1.0f;
		}
	}

	void Draw(SDL_Renderer* mRenderer) {
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

		SDL_Rect ball{
			static_cast<int>(mPos.x - mSize / 2),
			static_cast<int>(mPos.y - mSize / 2),
			mSize,
			mSize
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}

private:
	Vec2D randomVelocity() {
		int randomXDir = (rand() % 2 == 0) ? -1 : 1;
		int randomYDir = (rand() % 2 == 0) ? -1 : 1;

		int randomXMag = rand() % maxVelocityMag;
		int randomYMag = rand() % maxVelocityMag;

		return Vec2D{ (float)(randomXDir * randomXMag), (float)(randomYDir * randomYMag) };
	}

	void bounceFromWalls(int width, int height) {
		// Top Wall
		if (mPos.y <= 0.0f && mVel.y < 0.0f) {
			mVel.y *= -1.0f;
		}
		// Bottom Wall
		if (mPos.y >= height && mVel.y > 0.0f) {
			mVel.y *= -1.0f;
		}
		// Left Wall
		if (mPos.x <= 0.0f && mVel.x < 0.0f) {
			mVel.x *= -1.0f;
		}
		// Right Wall
		if (mPos.x >= width && mVel.x > 0.0f) {
			mVel.x *= -1.0f;
		}
	}
};