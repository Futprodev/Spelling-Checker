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

typedef struct
{
	int x;
	int y;
	int w;
	int h;
} rect_t;


void FillRect(rect_t rect, u32 pixel_color, u32 *screen_pixels)
{
	SDL_assert(screen_pixels);

	for (int row = 0; row < rect.h; ++row)
	{
		for (int col = 0; col < rect.w; ++col)
		{
			screen_pixels[(row + rect.y)*SCREEN_WIDTH + col + rect.x] = pixel_color;
		}
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *win = SDL_CreateWindow("Spell Checker",
										SCREEN_WIDTH,
										SCREEN_HEIGHT,
										SDL_WINDOW_RESIZABLE);
	SDL_assert(win);

	SDL_Renderer *renderer = SDL_CreateRenderer(win, 0);
	SDL_assert(renderer);

	SDL_Texture *screen = SDL_CreateTexture(renderer,
											SDL_PIXELFORMAT_XRGB8888,
											SDL_TEXTUREACCESS_STREAMING,
											SCREEN_WIDTH,
											SCREEN_HEIGHT);
	SDL_assert(screen);

	u32 *screen_pixels = (u32*) calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u32));
	SDL_assert(screen_pixels);

	SDL_PixelFormat *format = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_XRGB8888);
	SDL_assert(format);

	rect_t square = { 0, 0, 30, 30 };
	u32 pixel_color = SDL_MapRGB(format, NULL, 0, 255, 0);
	square.x = (SCREEN_WIDTH - square.w)/2;
	square.y = (SCREEN_HEIGHT - square.h)/2;
	FillRect(square, pixel_color, screen_pixels);


	BOOL done = FALSE;

	BOOL pressed_up = FALSE;
	BOOL pressed_down = FALSE;
	BOOL pressed_left = FALSE;
	BOOL pressed_right = FALSE;

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
			case SDLK_UP:
				pressed_up = TRUE;
				break;
			case SDLK_DOWN:
				pressed_down = TRUE;
				break;
			case SDLK_LEFT:
				pressed_left = TRUE;
				break;
			case SDLK_RIGHT:
				pressed_right = TRUE;
				break;
			default:
				break;
			}
		}

		memset(screen_pixels, 0,SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(u32));

		if (pressed_up)
		{
			square.y -= 10;
			if (square.y < 0)
			{
				square.y += 0;
			}
			pressed_up = FALSE;
		} 
		
		if (pressed_down)
		{
			square.y += 10;
			pressed_down = FALSE;
		}

		if (pressed_left)
		{
			square.x -= 10;
			pressed_left = FALSE;
		}

		if (pressed_right)
		{
			square.x += 10;
			pressed_right = FALSE;
		}

		FillRect(square, pixel_color, screen_pixels);

		SDL_UpdateTexture(screen, NULL, screen_pixels, SCREEN_WIDTH * sizeof(u32));
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, screen, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_Delay(14);
	}

	return 0;
}