#include <iostream>
#include <SDL.h>

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int, char**){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == NULL){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}


	SDL_RenderClear(ren);


	SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_Rect rect;
	rect.x=10;
	rect.y=10;
	rect.h=25;
	rect.w=25;


	if (SDL_RenderFillRect(ren, &rect))
	{
		std::cout << "SDL_RenderDrawRect Error: " << SDL_GetError() << std::endl;
	}
	SDL_RenderPresent(ren);



    SDL_Event e;
    bool quit = false;
    while (!quit){
    	while (SDL_PollEvent(&e)){
    		if (e.type == SDL_QUIT){
    			quit = true;
    		}
    		// if (e.type == SDL_KEYDOWN){
    		// 	quit = true;
    		// }
    		// if (e.type == SDL_MOUSEBUTTONDOWN){
    		// 	quit = true;
    		// }
            // SDL_Delay(100);
    	}
    }



	SDL_Quit();
	return 0;
}
