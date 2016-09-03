#include <iostream>
#include <SDL.h>


#include "led_anim.hpp"

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

    #define LED_COUNT 100
    led_anim_c<LED_COUNT> led_anim;

    for (int led=0; led<10; led++)
    {
        led_anim.fade_to_fast(led, CRGB(255,128,0), led+1);
        led_anim.fade_from_fast(led+10, CRGB(255,128,255), led+1);
    }


    SDL_Event e;
    bool quit = false;
    while (!quit){

        led_anim.step();

        //draw leds
        for (uint16_t led=0; led<LED_COUNT; led++)
        {
        	SDL_SetRenderDrawColor(ren, led_anim.led_level[led].r, led_anim.led_level[led].g, led_anim.led_level[led].b, SDL_ALPHA_OPAQUE);
        	SDL_Rect rect;
        	rect.x=(led+1)*12;
        	rect.y=12;
        	rect.h=10;
        	rect.w=10;


        	if (SDL_RenderFillRect(ren, &rect))
        	{
        		std::cout << "SDL_RenderDrawRect Error: " << SDL_GetError() << std::endl;
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
