#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class Menu {
public:
    Menu(SDL_Renderer* renderer, TTF_Font* font);
    ~Menu();

    void show(bool& quit, bool& startGame);

private:

    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* backgroundTexture;  
};

#endif