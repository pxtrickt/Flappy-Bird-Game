#include "menu.h"

Menu::Menu(SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font), backgroundTexture(nullptr) {
    // Load background image for the menu screen
    backgroundTexture = IMG_LoadTexture(renderer, "C:/Users/p4tri/FlappyBirdAssets/menu.png");
    if (!backgroundTexture) {
        std::cerr << "Failed to load textures! SDL_image Error: " << IMG_GetError() << std::endl;
    }
}

Menu::~Menu() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
}

void Menu::show(bool& quit, bool& startGame) {
    SDL_Event e;

    while (!quit && !startGame) {
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) startGame = true;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (backgroundTexture) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);  // Draw background
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}