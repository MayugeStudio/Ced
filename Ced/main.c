#include <stdio.h>
#include <SDL3/SDL.h>

static const int WIN_WIDTH = 1080;
static const int WIN_HEIGHT = 600;
static const char* WIN_TITLE = "Ced";

int main(int argc, char** argv) {
	SDL_Window* window = SDL_CreateWindow(WIN_TITLE, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        fprintf(stderr, "ERROR: could not create window\n");
        exit(1);
    }
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        fprintf(stderr, "ERROR: could not create renderer\n");
        SDL_DestroyWindow(window);
        exit(1);
    }

    SDL_Event ev;
	while (true) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_EVENT_QUIT)
                return 0;
        }
        SDL_RenderPresent(renderer);
	}

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}