//----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
 
//----------------------------------------------------------------------
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
 
//----------------------------------------------------------------------
#define SCREEN_W 640
#define SCREEN_H 480
#define SCREEN_SCALE 1
#define SCREEN_NAME "Who want to be millionaire"
#define BACKGROUND "bg.bmp"
#define BTN_START "btn_start.bmp"

void game_init(void);
void game_quit(void);

struct {
	// define "attributes"
	SDL_bool running;
	struct {
		unsigned int w;
		unsigned int h;
		const char* name;
		SDL_Window* Main_Window;
		SDL_Renderer* Main_Renderer;
	} screen;
    // define "methods"
	void (*init)(void);
	void (*quit)(void);
} Game = {
	SDL_FALSE,
	{
		SCREEN_SCALE*SCREEN_W,
		SCREEN_SCALE*SCREEN_H,
		SCREEN_NAME,
		NULL,
		NULL
	},
    game_init,
	game_quit
};

//----------------------------------------------------------------------
void game_init(void) {
	if(SDL_Init(SDL_INIT_EVERYTHING)!=0) {
		printf("SDL error -> %sn", SDL_GetError());
		exit(1);
	}

	unsigned int w = Game.screen.w;
	unsigned int h = Game.screen.h;
	const char* name = Game.screen.name;

	Game.screen.Main_Window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h, 0
	);
	Game.screen.Main_Renderer = SDL_CreateRenderer(
		Game.screen.Main_Window, -1,
		SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
	);

    Game.running = SDL_TRUE;
}

//----------------------------------------------------------------------
void game_quit(void) {
	SDL_DestroyRenderer(Game.screen.Main_Renderer);
	SDL_DestroyWindow(Game.screen.Main_Window);
	Game.screen.Main_Window = NULL;
	Game.screen.Main_Renderer = NULL;
 
	SDL_Quit();
}
 
 
//----------------------------------------------------------------------
int main(int argc, char* argv[]) {
	Game.init();
	SDL_Event event;
	while(Game.running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: {
					Game.running = SDL_FALSE;
				} break;
			}
		}
 
		SDL_RenderClear(Game.screen.Main_Renderer);

		SDL_Surface* Loading_Surf;
		Loading_Surf = SDL_LoadBMP("bg.bmp");
		SDL_Texture* Background_Tx = SDL_CreateTextureFromSurface(Game.screen.Main_Renderer, Loading_Surf);
		SDL_FreeSurface(Loading_Surf);
		SDL_RenderCopy(Game.screen.Main_Renderer, Background_Tx, NULL, NULL);
		
		SDL_RenderPresent(Game.screen.Main_Renderer);
	}
 
	Game.quit();
 
	return 0;
}
