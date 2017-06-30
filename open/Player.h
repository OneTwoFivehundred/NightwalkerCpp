#pragma once

#define DEFAULT 1

class Player {
public:
	SDL_Texture* texture;
	SDL_Rect pngData = { 0, 0 };
	SDL_Rect drawSprite = { 0, 0 };
	SDL_Rect position = { 200, 100 };

	//movement
	const Uint8* keystate;

	//a litte typedef to call getTexture
	typedef SDL_Texture*(*getTexture)(const char*, SDL_Renderer*, uint8_t);

	//sets the render
	Player(SDL_Renderer* render, size_t frames, getTexture getTexture, size_t height = 1) {
		texture = getTexture(R"(gfx\human-idle.png)", render, 
			DEFAULT);
		SDL_QueryTexture(texture, NULL, NULL, &pngData.w, &pngData.h);
		drawSprite.w = pngData.w / frames;
		drawSprite.h = pngData.h / height;

		position.w = drawSprite.w;
		position.h = drawSprite.h;
	};

	void updateAnim(float* delta) {
		if (*delta / 5 >= 0.1) {
			if ((drawSprite.x += drawSprite.w) == pngData.w) {
				drawSprite.x = 0;
			}

			if ((drawSprite.y += drawSprite.h) == pngData.h) {
				drawSprite.y = 0;
			}
			else if ((drawSprite.y += drawSprite.h) != pngData.h &&
				drawSprite.x == pngData.w)
			{
				drawSprite.y += drawSprite.h;
			}
			
			*delta = 0.0f;
	}}

	void updatePos(SDL_RendererFlip *flip, size_t boxes, ...) {
		SDL_Rect collision;
		va_list list;
		va_start(list, boxes);
		
		for (size_t i = 0; i < boxes; ++i) {
			collision = va_arg(list, SDL_Rect);
			if (collision.x + collision.w < position.x || collision.x > position.x + position.w ||
				collision.y + collision.h < position.y || collision.y > position.y + position.h) {
				++position.y;
			}
		}

		keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_S] ||
			keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_D]) {
			if (keystate[SDL_SCANCODE_W]) {
				//jump?
			} if (keystate[SDL_SCANCODE_S]) {
				//duck?
			} if (keystate[SDL_SCANCODE_A]) {
				*flip = SDL_FLIP_NONE;
				--position.x;
			} if (keystate[SDL_SCANCODE_D]) {
				++position.x;
				*flip = SDL_FLIP_HORIZONTAL;
			}
		} 

	}

	virtual ~Player() {
		//printf("Destroyed");
		SDL_DestroyTexture(texture);
		texture = nullptr;
	};
};