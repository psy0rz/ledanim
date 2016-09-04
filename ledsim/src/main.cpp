#include <iostream>
#include <SDL.h>


#include "strip_anim.hpp"

#include <iostream>

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int, char**){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Led strip simulation", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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




    //clear
    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren);

    #define COLS 50
    #define ROWS 30
    #define LED_COUNT ROWS*COLS

    strip_anim_c<LED_COUNT> strip_anim;

    strip_anim.commands.push_back(CMD_LED_SET_NEXT);
    strip_anim.commands.push_back(255);
    strip_anim.commands.push_back(0);
    strip_anim.commands.push_back(0);

    strip_anim.commands.push_back(CMD_LED_SET_NEXT);
    strip_anim.commands.push_back(0);
    strip_anim.commands.push_back(255);
    strip_anim.commands.push_back(0);

    strip_anim.commands.push_back(CMD_LED_SET_NEXT);
    strip_anim.commands.push_back(0);
    strip_anim.commands.push_back(0);
    strip_anim.commands.push_back(255);




    SDL_Event e;
    bool quit = false;
    while (!quit){

        strip_anim.step();

        //draw leds

        for (uint16_t row=0; row<ROWS; row++)
        {
            for (uint16_t col=0; col<COLS; col++)
            {
                uint16_t led=(row*COLS) + col;

            	SDL_SetRenderDrawColor(
                    ren,
                    strip_anim.led_anim.led_level[led].r,
                    strip_anim.led_anim.led_level[led].g,
                    strip_anim.led_anim.led_level[led].b,
                    SDL_ALPHA_OPAQUE
                );
            	SDL_Rect rect;
                rect.x=(col+1)*12;
                rect.y=(row+1)*12;
            	rect.h=10;
            	rect.w=10;


            	if (SDL_RenderFillRect(ren, &rect))
            	{
            		std::cout << "SDL_RenderDrawRect Error: " << SDL_GetError() << std::endl;
            	}

            }
        }

        //this loop basicly runs at monitor refresh speed
        SDL_RenderPresent(ren);

        while (SDL_PollEvent(&e))
        {
    		if (e.type == SDL_QUIT){
    			quit = true;
    		}
        }

    }



	SDL_Quit();
	return 0;
}
