#define SDL_MAIN_HANDLED

//libs
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#pragma comment(lib, "SDL2_ttf.lib")
#pragma comment(lib, "SDL2main.lib")

//SDL
#include "SDL_headers/SDL.h"
#include "SDL_headers/SDL_image.h"
#include "SDL_headers/SDL_ttf.h"
//#include "SDL_mixer.h"

#include <stdio.h>
#include <array>
#include <algorithm>
#include <cstdarg>

//Windows
#include <Windows.h>
#include <Lmcons.h>

//Classes
#include "Player.h"
#include "Enemies.h"

//defines
#define DEFAULT 1

inline SDL_Texture* getTexture(const char* filename, SDL_Renderer* render, uint8_t type)
{
	SDL_Surface* surface = IMG_Load(filename);
	SDL_Texture* texture = nullptr;
	if (surface == NULL) {
		fprintf(stderr, "IMG_Load() failed, description: %s\n", IMG_GetError());
		return NULL;
	}

	switch (type)
	{
	case NULL:
		break;

	case DEFAULT:
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00, 0x9E));
		break;

	default:
		throw std::out_of_range("'type' is out of range");
	}

	texture = SDL_CreateTextureFromSurface(render, surface);
	if (texture == NULL) {
		fprintf(stderr, "SDL_CreateTextureFromSurface() failed, description: %s\n",
			IMG_GetError());
		return NULL;
	}

	//clean up
	SDL_FreeSurface(surface);
	surface = nullptr;

	return texture;
}

//Global Vars
char username[UNLEN + 1];

int main(int argc, char* argv[])
{
	//Vars
	SDL_Renderer* render = nullptr;
	SDL_Window* window = nullptr;
	SDL_Event events;

	//Username
	DWORD usernameLen = UNLEN + 1;
	GetUserNameA(username, &usernameLen);

	//Beta
	SDL_Texture* ground = nullptr;
	SDL_Rect groundRect = { 0, 0, 31, 23 };
	SDL_Rect groundPos = { 200, 200, 31, 23 };

	//Init stage
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Init() failed, description: %s\n", SDL_GetError());
		return -1;
	}

	if (TTF_Init() != 0) {
		fprintf(stderr, "TTF_Init failed, description: %s\n", TTF_GetError());
		return -1;
	}

	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags) {
		fprintf(stderr, "SDL_Init() failed, description: %s\n", IMG_GetError());
		return -1;
	}

	//creates window & renderers
	window = SDL_CreateWindow("Nightwalker v0.01", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//classes
	Player player(render, 5, getTexture);

	//beta
	ground = getTexture(R"(gfx\terrain.png)", render, DEFAULT);

	//Sets background
	SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
	bool isAlive = true;

	//for animating
	int prev = 0;
	int now = 0;
	float delta = 0;
	float frameTime = 0;
	SDL_RendererFlip flipStatus;

	while (isAlive)
	{
		//for animating
		prev = now;
		now = SDL_GetTicks();
		delta = (now - prev) / 1000.0f;
		if (SDL_PollEvent(&events) != 0)
			switch (events.type)
			{
			case SDL_KEYDOWN:
			switch (events.key.keysym.sym)
			case SDLK_ESCAPE:
				isAlive = false;
				break;

			case SDL_QUIT:
				isAlive = false;
				break;
			}

		frameTime += delta;
		player.updateAnim(&frameTime);
		
		player.updatePos(&flipStatus, 1, groundPos);

		SDL_RenderClear(render);
		SDL_RenderCopyEx(render, player.texture, &player.drawSprite, &player.position,
			NULL, NULL, flipStatus);
		SDL_RenderCopy(render, ground, &groundRect, &groundPos);
		SDL_RenderPresent(render);
	}

	SDL_DestroyRenderer(render);
	render = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}