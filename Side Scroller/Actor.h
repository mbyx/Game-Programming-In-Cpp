#pragma once

#include "Vec2D.h"
#include "Component.h"
#include <vector>
#include <algorithm>

class Actor {
public:
	enum State {
		Active, Paused, Dead
	};

	Actor(class Game* game) {
		mGame = game;
		mPosition = Vec2D{ 0.0, 0.0 };
		mScale = 1.0;
		mRotation = 0.0;
		mState = Active;
	}

	virtual ~Actor() {
		for (auto component : mComponents) {
			RemoveComponent(component);
			delete component;
		}
	}

	void Update(float delta) {
		if (mState == Active) {
			UpdateComponents(delta);
			UpdateActor(delta);
		}
	}

	void UpdateComponents(float delta) {
		// Larger update order will update faster.
		std::sort(mComponents.begin(), mComponents.end(), [](Component* const a, Component* const b) {
			return a->GetUpdateOrder() < b->GetUpdateOrder();
			});
		for (auto component : mComponents) {
			component->Update(delta);
		}
	}

	virtual void UpdateActor(float delta) {}

	State GetState() {
		return mState;
	}

	Vec2D GetPosition() {
		return mPosition;
	}

	float GetScale() {
		return mScale;
	}

	float GetRotation() {
		return mRotation;
	}

	State SetState(State newState) {
		State oldState = mState;
		mState = newState;
		return oldState;
	}

	Vec2D SetPosition(Vec2D newPosition) {
		Vec2D oldPosition = mPosition;
		mPosition = newPosition;
		return oldPosition;
	}

	float SetScale(float newScale) {
		float oldScale = mScale;
		mScale = newScale;
		return oldScale;
	}

	float SetRotation(float newRotation) {
		float oldRotation = mRotation;
		mRotation = newRotation;
		return oldRotation;
	}

	void AddComponent(Component* component) {
		mComponents.emplace_back(component);
	}

	void RemoveComponent(Component* component) {
		std::vector<Component*>::iterator position = std::find(mComponents.begin(), mComponents.end(), component);
		mComponents.erase(position);
	}

private:
	State mState;
	Vec2D mPosition;
	float mScale;
	float mRotation;

	std::vector<Component*> mComponents;
	class Game* mGame;
};