#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "Actor.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "BackgroundSprite.h"
#include "Ship.h"
#include "AssetManager.h"

#include <vector>
#include <algorithm>
#include <iostream>

class Game {
public:
	Game() {
		mWindow = nullptr;
		mRenderer = nullptr;
		mIsRunning = true;
		mTicksCount = 0;
		mUpdatingActors = false;
		mAssets = new AssetManager("Assets/");
	}

	// https://stackoverflow.com/questions/30629106/sdl2-how-to-properly-toggle-fullscreen
	void ToggleFullscreen(SDL_Window* Window) {
		Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
		bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
		SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
		SDL_ShowCursor(IsFullscreen);
	}

	bool Initialize(const char* windowName, Vec2D windowPosition, Vec2D windowSize) {
		int sdlResult = SDL_Init(SDL_INIT_VIDEO);
		if (sdlResult != 0) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}

		mWindow = SDL_CreateWindow(windowName, windowPosition.x, windowPosition.y, windowSize.x, windowSize.y, 0);
		if (!mWindow) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return false;
		}

		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!mRenderer) {
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			return false;
		}

		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

		LoadData();

		return true;
	}

	void Run() {
		while (mIsRunning) {
			Process();
			Update();
			Draw();
		}
	}

	void Shutdown() {
		while (!mActors.empty()) {
			Actor* actor = mActors.back();
			RemoveActor(actor);
			delete actor;
		}

		IMG_Quit();
		SDL_DestroyWindow(mWindow);
		SDL_DestroyRenderer(mRenderer);
		SDL_Quit();
	}

	void AddActor(class Actor* actor) {
		if (mUpdatingActors)
			mPendingActors.emplace_back(actor);
		else
			mActors.emplace_back(actor);
	}

	void RemoveActor(Actor* actor) {
		std::vector<Actor*>::iterator pendingPos = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		std::vector<Actor*>::iterator actorPos = std::find(mActors.begin(), mActors.end(), actor);

		if (pendingPos != mPendingActors.end()) {
			mPendingActors.erase(pendingPos);
		}
		else if (actorPos != mActors.end()) {
			mActors.erase(actorPos);
		}
	}

private:
	void LoadData() {
		mShip = new Ship(this);
		mShip->SetPosition(Vec2D { 100.0f, 384.0f });
		mShip->SetScale(1.5f);

		AnimatedSprite* as = new AnimatedSprite(mShip);
		std::vector<SDL_Texture*> animations = {
			mAssets->LoadTexture("Ship01.png", mRenderer),
			mAssets->LoadTexture("Ship02.png", mRenderer),
			mAssets->LoadTexture("Ship03.png", mRenderer),
			mAssets->LoadTexture("Ship04.png", mRenderer),
		};
		as->SetAnimationTextures(animations);

		Actor* actor = new Actor{this};
		actor->SetPosition(Vec2D{ 512.0f, 384.0f });
		BackgroundSprite* bg = new BackgroundSprite{ actor };
		bg->SetScreenSize(Vec2D{ 1024.0f, 768.0f });
		std::vector<SDL_Texture*> bgtexs = {
			mAssets->LoadTexture("Farback01.png", mRenderer),
			mAssets->LoadTexture("Farback02.png", mRenderer)
		};
		bg->SetBackgroundTextures(bgtexs);
		bg->SetScrollSpeed(-100.0f);
		mAssets->AddSprite(bg);

		bg = new BackgroundSprite(actor, 50);
		bg->SetScreenSize(Vec2D{ 1024.0f, 768.0f });
		bgtexs = {
			mAssets->LoadTexture("Stars.png", mRenderer),
			mAssets->LoadTexture("Stars.png", mRenderer)
		};
		bg->SetBackgroundTextures(bgtexs);
		bg->SetScrollSpeed(-200.0f);
		mAssets->AddSprite(bg);
		AddActor(actor);
		AddActor(mShip);
		mAssets->AddSprite(as);
	}

	void Process() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				mIsRunning = false;
				break;
			}
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE]) {
			mIsRunning = false;
		}

		if (state[SDL_SCANCODE_F]) {
			ToggleFullscreen(mWindow);
		}

		mShip->ProcessKeyboard(state);
	}

	void Update() {
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
		float delta = (SDL_GetTicks() - mTicksCount) / 1000.0f;
		if (delta > 0.05f) {
			delta = 0.05f;
		}
		mTicksCount = SDL_GetTicks();

		mUpdatingActors = true;
		for (auto actor : mActors) {
			actor->Update(delta);
		}
		mUpdatingActors = false;

		for (auto pendingActor : mPendingActors) {
			mPendingActors.emplace_back(pendingActor);
		}
		mPendingActors.clear();

		std::vector<Actor*> deadActors;
		for (auto actor : mActors) {
			if (actor->GetState() == Actor::Dead) {
				deadActors.emplace_back(actor);
			}
		}

		for (auto actor : deadActors) {
			delete actor;
		}
	}

	void Draw() {
		SDL_SetRenderDrawColor(mRenderer, 18, 18, 18, 128);
		SDL_RenderClear(mRenderer);

		mAssets->Draw(mRenderer);

		SDL_RenderPresent(mRenderer);
	}

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	AssetManager* mAssets;

	Uint32 mTicksCount;
	bool mIsRunning;
	
	std::vector<Actor*> mActors;
	std::vector<Actor*> mPendingActors;
	bool mUpdatingActors;

	class Ship* mShip;
};

int main(int argc, char** argv) {
	Game game;

	bool success = game.Initialize("Side Scroller", Vec2D{ 25.0f, 25.0f }, Vec2D{ 1024.0f, 768.0f });
	if (success) {
		game.Run();
	}
	game.Shutdown();

	return 0;
}