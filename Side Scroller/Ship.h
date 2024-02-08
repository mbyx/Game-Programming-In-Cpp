#pragma once
#include "Actor.h"
#include "AnimatedSprite.h"

class Ship : public Actor {
public:
	Ship(class Game* game) : Actor(game) {
		mRightSpeed = 0.0f;
		mDownSpeed = 0.0f;
	}

	void UpdateActor(float delta) override {
		Actor::UpdateActor(delta);
		Vec2D position = GetPosition();
		position.x += mRightSpeed * delta;
		position.y += mDownSpeed * delta;

		if (position.x < 25.0f)
		{
			position.x = 25.0f;
		}
		else if (position.x > 500.0f)
		{
			position.x = 500.0f;
		}
		if (position.y < 25.0f)
		{
			position.y = 25.0f;
		}
		else if (position.y > 743.0f)
		{
			position.y = 743.0f;
		}

		SetPosition(position);
	}

	void ProcessKeyboard(const Uint8* state) {
		mDownSpeed = mRightSpeed = 0;
		if (state[SDL_SCANCODE_W]) {
			mDownSpeed -= 300.0f;
		}
		if (state[SDL_SCANCODE_S]) {
			mDownSpeed += 300.0f;
		}
		if (state[SDL_SCANCODE_A]) {
			mRightSpeed -= 250.0f;
		}
		if (state[SDL_SCANCODE_D]) {
			mRightSpeed += 250.0f;
		}
	}

	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }

private:
	float mRightSpeed;
	float mDownSpeed;
};