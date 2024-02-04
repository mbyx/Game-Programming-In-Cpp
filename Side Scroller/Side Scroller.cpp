#include "SDL.h"
#include "SDL_image.h"
#include "Actor.h"
#include "Sprite.h"

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
	}

	// https://stackoverflow.com/questions/30629106/sdl2-how-to-properly-toggle-fullscreen
	void ToggleFullscreen(SDL_Window* Window) {
		Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
		bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
		SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
		SDL_ShowCursor(IsFullscreen);
	}

	bool Initialize() {
		int sdlResult = SDL_Init(SDL_INIT_VIDEO);
		if (sdlResult != 0) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}

		mWindow = SDL_CreateWindow("Side Scroller", 25, 25, 1024, 768, 0);
		if (!mWindow) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return false;
		}

		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!mRenderer) {
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			return false;
		}

		IMG_Init(IMG_INIT_PNG);

		LoadData();

		return true;
	}

	void RunLoop() {
		while (mIsRunning) {
			ProcessInput();
			UpdateGame();
			GenerateOutput();
		}
	}

	void Shutdown() {
		while (!mActors.empty()) {
			delete mActors.back();
		}

		while (!mSprites.empty()) {
			delete mSprites.back();
		}

		SDL_DestroyWindow(mWindow);
		SDL_DestroyRenderer(mRenderer);
		SDL_Quit();
	}

private:
	void LoadData() {
		Actor* actor = new Actor{this};
		actor->SetPosition(Vec2D{ 512, 384 });
		Sprite* sprite = new Sprite{ actor };
		sprite->SetTexture(LoadTexture("C:\\Users\\Muiz Parker\\OneDrive\\Pictures\\Persona 4 Golden\\Logo.png"));
		actor->AddComponent(sprite);
		AddActor(actor);
		AddSprite(sprite);
	}

	void AddSprite(Sprite* sprite) {
		mSprites.emplace_back(sprite);
		// Largest draw order goes first.
		std::sort(mSprites.begin(), mSprites.end(), [](Sprite* const a, Sprite* const b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
		});
	}

	void ProcessInput() {
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
	}

	void UpdateGame() {
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

	void GenerateOutput() {
		SDL_SetRenderDrawColor(mRenderer, 18, 18, 18, 128);
		SDL_RenderClear(mRenderer);

		for (auto sprite : mSprites) {
			sprite->Draw(mRenderer);
		}

		SDL_RenderPresent(mRenderer);
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
		} else if (actorPos != mActors.end()) {
			mActors.erase(actorPos);
		}
	}

	SDL_Texture* LoadTexture(const char* fileName) {
		SDL_Surface* surface = IMG_Load(fileName);
		if (!surface) {
			SDL_Log("Failed to load texture file: %s", SDL_GetError());
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		SDL_FreeSurface(surface);
		if (!texture) {
			SDL_Log("Failed to convert surface to a texture: %s", SDL_GetError());
			return nullptr;
		}

		return texture;
	}

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;

	std::vector<Actor*> mActors;
	std::vector<Actor*> mPendingActors;
	std::vector<Sprite*> mSprites;
	bool mUpdatingActors;
};

int main(int argc, char** argv) {
	Game game;

	bool success = game.Initialize();
	if (success) {
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}