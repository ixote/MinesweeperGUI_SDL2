#include "game.c"
#include "manu.c"
// #include"various.h"
int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    MAIN_WINDOW = SDL_CreateWindow("扫雷v0.1",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        MANU_WIDTH, MANU_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS);
    SDL_SetWindowOpacity(MAIN_WINDOW, 0.9);
    MAIN_RENDER = SDL_CreateRenderer(MAIN_WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(MAIN_RENDER, SDL_BLENDMODE_BLEND);
    initFreeAdCur();
    initRe();
    int* init = manuWindow();
    Message* result;
    while (1) {
        if (init[0] == 0)
            break;
        else {
            SDL_Delay(100);
            result = Game(init);
        }
        if (result->next == false)
            break;
        else {
            SDL_Delay(100);
            init = manuWindow();
        }
    }
    SDL_DestroyRenderer(MAIN_RENDER);
    SDL_DestroyWindow(MAIN_WINDOW); //销毁window
    SDL_Quit(); // 卸载SDL
    freeGame();
    return 0;
}