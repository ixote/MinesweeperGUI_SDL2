#ifndef _IMAGE_H
#define _IMAGE_H
#include"main.h"

#define INITVAR(name) SDL_Texture* name=NULL
#define INITIMAGE(name) name=initText("res/" #name ".png\n",render)
#define FREEIMAG(name) SDL_DestroyTexture(name);

#define IMAGE_LIST(_) \
    _(num_1);         \
    _(num_2);         \
    _(num_3);         \
    _(num_4);         \
    _(num_5);         \
    _(num_6);         \
    _(num_7);         \
    _(num_8);         \
    _(blank);         \
    _(hidden);        \
    _(found);         \
    _(wrong);         \
    _(time_0);        \
    _(time_1);        \
    _(time_2);        \
    _(time_3);        \
    _(time_4);        \
    _(time_5);        \
    _(time_6);        \
    _(time_7);        \
    _(time_8);        \
    _(time_9)

SDL_Texture* initText(char *path,SDL_Renderer *render);

SDL_Texture* initText(char *path,SDL_Renderer *render){
    SDL_Surface *image = IMG_Load(path);
    SDL_Surface *opt_image = (SDL_Surface *)malloc(sizeof(SDL_Surface));
    SDL_Texture *text = SDL_CreateTextureFromSurface(render,opt_image);
    SDL_FreeSurface(opt_image);
    SDL_FreeSurface(image);
    // SDL_RenderCopy(render,text,NULL,&pot->rect);
    return text;
}

#endif