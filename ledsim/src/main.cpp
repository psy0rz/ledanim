#include <iostream>
#include <SDL.h>


#include "strip_anim.hpp"

#include <iostream>

#include <time.h>

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
    // SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED );
    if (ren == NULL){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }




    //clear
    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren);

    #define COLS 40
    #define ROWS 4
    #define LED_COUNT ROWS*COLS

    strip_anim_c<LED_COUNT> strip_anim;


    //random noise to test various functions
    strip_anim.commands={
        CMD_REPEAT_BEGIN_RND   , 0,0  , 0,10  ,
        CMD_PEN_COLOR_RND      , 0,255 , 0,255 , 0,255 ,
        CMD_PEN_FADE_SPEED_RND , 1 , 10 ,
        CMD_PEN_WIDTH_RND      , 1 , 10 ,
        CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
        CMD_PEN_FADE_MODE      , FADE_FROM_FAST ,
        CMD_PEN_DRAW           ,
        CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
        CMD_PEN_DRAW           ,
        CMD_REPEAT_END         ,
    };

    //fire animation, we try to make the indirect light on the ground look like fire.
    strip_anim.commands={
        //glowing red background
        CMD_REPEAT_BEGIN       , 0,100,
        CMD_PEN_COLOR_RND      , 20,127 , 0,0 , 0,0 ,
        CMD_PEN_FADE_SPEED     , 1  ,
        CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
        CMD_PEN_WIDTH          , 1,
        CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
        CMD_PEN_DRAW           ,
        CMD_REPEAT_END         ,

        //bright yellow flames
        CMD_REPEAT_BEGIN       , 0,2,
        CMD_PEN_COLOR_RND      , 200,255 , 100,200 , 0,0 ,
        CMD_PEN_FADE_SPEED_RND , 10 , 30 ,
        CMD_LED_NR_16_RND      , 0,0 , 255,255 ,
        CMD_PEN_WIDTH_RND      , 1 , 3,
        CMD_PEN_FADE_MODE      , FADE_TO_FAST ,
        CMD_PEN_DRAW           ,
        CMD_REPEAT_END         ,
    };


    //KITT knightrider radar
    strip_anim.commands={
        CMD_PEN_COLOR          , 255 , 0 , 0 ,
        CMD_PEN_FADE_SPEED     , 5  ,
        CMD_PEN_FADE_MODE      , FADE_FROM_FAST ,
        CMD_PEN_CLONE_COUNT    , 0,3 ,
        CMD_PEN_CLONE_OFFSET   , 0,9 ,

        //to the right
        CMD_PEN_STEP           , 1 ,
        CMD_REPEAT_BEGIN       , 0,6,
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

    //police lights
    strip_anim.commands={
        CMD_PEN_WIDTH          , 10,
        CMD_PEN_CLONE_COUNT    , 0,1 ,
        CMD_PEN_CLONE_OFFSET   , 0,20,

        ///////////////////////// left red


        //left red short
        CMD_LED_NR_8           , 0,
        CMD_PEN_COLOR          , 255 , 0 , 0 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 4,

        //left off
        CMD_LED_NR_8           , 0,
        CMD_PEN_COLOR          , 0 , 0 , 0 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 2,


        //left red long
        CMD_LED_NR_8           , 0,
        CMD_PEN_COLOR          , 255 , 0 , 0 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 14,

        //left off
        CMD_LED_NR_8           , 0,
        CMD_PEN_COLOR          , 0 , 0 , 0 ,
        CMD_PEN_DRAW           ,

        ///////////////////////// right blue

        //right blue short
        CMD_LED_NR_8           , 10,
        CMD_PEN_COLOR          , 0, 0 , 255 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 4,

        //right off
        CMD_LED_NR_8           , 10,
        CMD_PEN_COLOR          , 0 , 0 , 0 ,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 2,


        //right blue long
        CMD_LED_NR_8           , 10,
        CMD_PEN_COLOR          , 0 , 0 , 255,
        CMD_PEN_DRAW           ,
        CMD_DELAY_8            , 14,

        //right off
        CMD_LED_NR_8           , 10,
        CMD_PEN_COLOR          , 0 , 0 , 0 ,
        CMD_PEN_DRAW           ,


    };


    struct timespec now;
    struct timespec prev;
    int count=0;

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

        // FPS
        count++;
        clock_gettime(CLOCK_REALTIME,&now);
        if (now.tv_sec!=prev.tv_sec)
        {
            std::cout << "fps: " << count << "\n";
            count=0;
            prev.tv_sec=now.tv_sec;
        }


    }



    SDL_Quit();
    return 0;
}
