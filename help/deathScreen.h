#ifndef DEAD_H
#define DEAD_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <SDL_mixer.h>

class Death {
public:
    Death(SDL_Renderer* renderer);
    ~Death();

    void show(bool& quit, bool& startGame);

private:

    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;  
    Mix_Chunk* deathSound = nullptr;
};

#endif