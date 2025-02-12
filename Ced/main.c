#include <stdio.h>
#include "SDL3/SDL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIN_WIDTH 1080
#define WIN_HEIGHT 600
#define WIN_TITLE "Ced"

#define FONT_SPRITESHEET_WIDTH 112
#define FONT_SPRITESHEET_HEIGHT 84

#define FONT_WIDTH 8
#define FONT_HEIGHT 12

#define FONT_SPRITESHEET_COLS 14
#define FONT_SPRITESHEET_ROWS 7

SDL_Surface* surface_from_file(const char *file_path)
{
    int w, h, n;
    unsigned char *pixels = stbi_load(file_path, &w, &h, &n, STBI_rgb_alpha);
    if (pixels == NULL) {
        fprintf(stderr, "ERROR: could not open load font file %s: %s\n",
            file_path, stbi_failure_reason());
        exit(1);
    }

    SDL_Surface *surface = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, pixels, 4 * w);
    if (surface == NULL) {
        fprintf(stderr, "ERROR: could not create surface: %s\n", stbi_failure_reason());
        stbi_image_free(pixels);
        exit(1);
    }
    return surface;
}

void render_char(SDL_Renderer* renderer, char c, SDL_Texture *font, int x, int y, float scale)
{
    // white space 32
    int index;
    if (c == ' ') {
        index = 14;
    }
    else {
        index = c - 33;
    }
    
    int col = index % FONT_SPRITESHEET_COLS;
    int row = index / FONT_SPRITESHEET_COLS;
    const SDL_FRect src = { .x = col * FONT_WIDTH, .y = row * FONT_HEIGHT, .w = FONT_WIDTH, .h = FONT_HEIGHT };
    const SDL_FRect dst = {
        .x = x,
        .y = y,
        .w = FONT_WIDTH * scale,
        .h = FONT_HEIGHT * scale,
    };

    SDL_RenderTextureTiled(renderer, font, &src, scale, &dst);
}

const char text[] = "Hello";

int main(int argc, char** argv) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "ERROR: could not initialize SDL2: %s\n", SDL_GetError());
        return 0;
    }

	SDL_Window* window = SDL_CreateWindow(WIN_TITLE, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        fprintf(stderr, "ERROR: could not create window: %s\n", SDL_GetError());
        exit(1);
    }
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        fprintf(stderr, "ERROR: could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        exit(1);
    }

    SDL_Surface *font = surface_from_file("font.png");
    SDL_Texture *font_texture = SDL_CreateTextureFromSurface(renderer, font);
    SDL_SetTextureScaleMode(font_texture, SDL_SCALEMODE_NEAREST);

    SDL_Event ev;

    float scale = 5.0f;

	while (true) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_EVENT_QUIT)
                goto exit;
        }
        for (int i = 0; i < strlen(text); ++i) {
            render_char(renderer, text[i], font_texture, i * FONT_WIDTH * scale, 40, scale);
        }
        SDL_RenderPresent(renderer);
	}

    exit:

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}