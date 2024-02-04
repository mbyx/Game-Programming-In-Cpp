#include "SDL.h"
#include "Ball.h"
#include "Paddle.h"
#include <vector>
#include <iostream>


class Game {
public:
	Game() {
		mWindow = nullptr;
		mRenderer = nullptr;
		mIsRunning = true;
		mTicksCount = 0;

		for (int i = 0; i < mNumberOfBalls; i++) {
			Ball randomBall{ 512, 384 };
			mBalls.push_back(randomBall);
		}
	}

	bool Initialize() {
		int sdlResult = SDL_Init(SDL_INIT_VIDEO);
		if (sdlResult != 0) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}

		mWindow = SDL_CreateWindow("Multiball", 25, 25, 1024, 768, 0);
		if (!mWindow) {
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return false;
		}

		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!mRenderer) {
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			return false;
		}

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
		SDL_DestroyWindow(mWindow);
		SDL_DestroyRenderer(mRenderer);
		SDL_Quit();
	}

private:
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
		
		mPaddleLeft.mDir = 0;
		mPaddleRight.mDir = 0;

		if (state[SDL_SCANCODE_UP]) {
			mPaddleRight.mDir -= 1;
		}
		else if (state[SDL_SCANCODE_DOWN]) {
			mPaddleRight.mDir += 1;
		}

		if (state[SDL_SCANCODE_W]) {
			mPaddleLeft.mDir -= 1;
		}
		else if (state[SDL_SCANCODE_S]) {
			mPaddleLeft.mDir += 1;
		}
	}

	void UpdateGame() {
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

		float delta = (SDL_GetTicks() - mTicksCount) / 1000.0f;
		if (delta > 0.05f) {
			delta = 0.05f;
		}
		mTicksCount = SDL_GetTicks();
		
		mPaddleLeft.Update(delta);
		mPaddleRight.Update(delta);

		for (Ball& ball : mBalls) {
			ball.Update(delta, mPaddleLeft, mPaddleRight);
		}
	}

	void GenerateOutput() {
		SDL_SetRenderDrawColor(mRenderer, 18, 18, 18, 128);
		SDL_RenderClear(mRenderer);

		for (Ball& ball : mBalls) {
			ball.Draw(mRenderer);
		}

		//std::cout << mPaddleRight.mPos.x << std::endl;
		mPaddleLeft.Draw(mRenderer);
		mPaddleRight.Draw(mRenderer);

		SDL_RenderPresent(mRenderer);
	}

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;

	std::vector<Ball> mBalls;
	int mNumberOfBalls = 20;

	Paddle mPaddleLeft{30, 384};
	Paddle mPaddleRight{ 1024 - 15, 384 };
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