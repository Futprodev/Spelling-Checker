#include <stdio.h>
#include <assert.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define BOOL u32
#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

typedef Uint32 u32;
typedef Uint64 u64;
typedef Sint32 i32;
typedef Sint64 i64;

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* win = SDL_CreateWindow("Spell Checker",
										SCREEN_WIDTH,
										SCREEN_HEIGHT,
										SDL_WINDOW_RESIZABLE);
	assert(win);

	SDL_Renderer* renderer = SDL_CreateRenderer(win, 0);
	assert(renderer);

	SDL_Texture* screen = SDL_CreateTexture(renderer,
											SDL_PIXELFORMAT_XRGB8888,
											SDL_TEXTUREACCESS_STREAMING,
											SCREEN_WIDTH,
											SCREEN_HEIGHT);
	assert(screen);

	u32 *screen_pixels = (u32*)calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u32));
	assert(screen_pixels);

	BOOL done = FALSE;

	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_Quit)
			{
				done = TRUE;
				break;
			}

			if (event.type != SDL_EVENT_KEY_DOWN)
			{
				break;
			}

			SDL_Keycode code = event.key.key;

			switch (code)
			{
			case SDLK_ESCAPE:
				done = TRUE;
				break;
			default:
				break;
			}

			SDL_UpdateTexture(screen, NULL, screen_pixels, NULL);
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, screen, NULL, NULL);
			SDL_RenderPresent(renderer);

			SDL_Delay(40);
		}
	}
}