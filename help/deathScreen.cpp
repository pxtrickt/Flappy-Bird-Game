#include "deathScreen.h"

Death::Death(SDL_Renderer* renderer) : renderer(renderer),  backgroundTexture(nullptr) {
    //loading background
    backgroundTexture = IMG_LoadTexture(renderer, "C:/Users/p4tri/FlappyBirdAssets/deathScreen.png");
    if (!backgroundTexture) {
        std::cerr << "Failed to load textures! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    //laoding sound
    deathSound = Mix_LoadWAV("C:\\Users\\p4tri\\FlappyBirdAssets\\death.wav");
    if (!deathSound) {
        std::cerr << "Failed to load death sound effect: " << Mix_GetError() << std::endl;
    }
}

Death::~Death() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
}

void Death::show(bool& quit, bool& startGame) {
    SDL_Event e;

    if (deathSound) {
        Mix_PlayChannel(-1, deathSound, 0);
    }
    // loop for whether user plays again or quits
    while (!quit && !startGame) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT ) quit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_n) quit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_y) startGame = true;
            
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