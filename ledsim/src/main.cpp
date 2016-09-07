#include <iostream>
#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#include "strip_anim.hpp"

#include <iostream>

#include <time.h>

#define COLS 10
#define ROWS 6
#define LED_COUNT ROWS*COLS

strip_anim_c<LED_COUNT> strip_anim;
SDL_Renderer *ren;
bool quit=false;

void step()
{
    SDL_Event e;
    struct timespec now;
    static struct timespec prev;
    static int count=0;

    // FPS
    count++;
    clock_gettime(CLOCK_REALTIME,&now);
    if (now.tv_sec!=prev.tv_sec)
    {
        std::cout << "fps: " << count << "\n";
        count=0;
        prev.tv_sec=now.tv_sec;
    }


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
            quit=true;
        }
    }



}

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

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED );
    if (ren == NULL){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }




    //clear
    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren);




    // //random noise to test various functions
    // strip_anim.commands={
    //     CMD_REPEAT_BEGIN_RND   , 0,0  , 0,10  ,
    //     CMD_PEN_COLOR_RND      , 0,255 , 0,255 , 0,255 ,
    //     CMD_PEN_FADE_SPEED_RND , 1 , 10 ,
    //     CMD_PEN_WIDTH_RND      , 1 , 10 ,
    //     CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
    //     CMD_PEN_FADE_MODE      , FADE_FROM_FAST ,
    //     CMD_PEN_DRAW           ,
    //     CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
    //     CMD_PEN_DRAW           ,
    //     CMD_REPEAT_END         ,
    // };

    // //fire animation, we try to make the indirect light on the ground look like fire.
    // strip_anim.commands={
    //     //glowing red background
    //     CMD_REPEAT_BEGIN       , 0,100,
    //     CMD_PEN_COLOR_RND      , 20,127 , 0,0 , 0,0 ,
    //     CMD_PEN_FADE_SPEED     , 1  ,
    //     CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
    //     CMD_PEN_WIDTH          , 1,
    //     CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
    //     CMD_PEN_DRAW           ,
    //     CMD_REPEAT_END         ,
    //
    //     //bright yellow flames
    //     CMD_REPEAT_BEGIN       , 0,2,
    //     CMD_PEN_COLOR_RND      , 200,255 , 100,200 , 0,0 ,
    //     CMD_PEN_FADE_SPEED_RND , 10 , 30 ,
    //     CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
    //     CMD_PEN_WIDTH_RND      , 1 , 3,
    //     CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
    //     CMD_PEN_DRAW           ,
    //     CMD_REPEAT_END         ,
    // };


    //KITT knightrider radar
    strip_anim.commands={
        CMD_PEN_COLOR          , 255 , 0 , 0 ,
        CMD_PEN_FADE_SPEED     , 5  ,
        CMD_PEN_FADE_MODE      , FADE_FROM_FAST ,
        CMD_PEN_CLONE_COUNT    , 0,3 ,
        CMD_PEN_CLONE_OFFSET   , 0,9 ,

        //to the right
        CMD_PEN_STEP           , 1 ,
        CMD_REPEAT_BEGIN       , 0,6 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 8 ,
        CMD_REPEAT_END         ,

        //to the left
        CMD_PEN_STEP           , (uint8_t)-1 ,
        CMD_REPEAT_BEGIN       , 0,6,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 8 ,
        CMD_REPEAT_END         ,
    };
    //
    // //police lights
    // strip_anim.commands={
    //     CMD_PEN_WIDTH          , 10,
    //     CMD_PEN_CLONE_COUNT    , 0,5 ,
    //     CMD_PEN_CLONE_OFFSET   , 0,20,
    //
    //     ///////////////////////// left red
    //
    //
    //     //left red short
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 255 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 4,
    //
    //     //left off
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 2,
    //
    //
    //     //left red long
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 255 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 14,
    //
    //     //left off
    //     CMD_LED_NR_8           , 0,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //
    //     ///////////////////////// right blue
    //
    //     //right blue short
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0, 0 , 255 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 4,
    //
    //     //right off
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 2,
    //
    //
    //     //right blue long
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0 , 0 , 255,
    //     CMD_PEN_DRAW           ,
    //     CMD_DELAY_8            , 14,
    //
    //     //right off
    //     CMD_LED_NR_8           , 10,
    //     CMD_PEN_COLOR          , 0 , 0 , 0 ,
    //     CMD_PEN_DRAW           ,
    //
    //
    // };



    // //pure performance test without graphics:
    // while(1)
    // {
    //     strip_anim.step();
    //     count++;
    //     clock_gettime(CLOCK_REALTIME,&now);
    //     if (now.tv_sec!=prev.tv_sec)
    //     {
    //         std::cout << "step/s: " << count << "\n";
    //         count=0;
    //         prev.tv_sec=now.tv_sec;
    //     }
    // }



#ifdef __EMSCRIPTEN__
      // void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
      emscripten_set_main_loop(step, 60, 1);
#else
    //main loop
    while(!quit) step() ;
#endif



    SDL_Quit();
    return 0;
}
