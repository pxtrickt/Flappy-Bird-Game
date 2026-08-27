#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "sdlManager.h"
#include "objects.h"

class Gameplay {
public:
    Gameplay(SDL_Renderer* renderer) : renderer(renderer), squirrel(100, SCREEN_HEIGHT / 2, 50, 50) {
        backgroundTexture = TextureManager::loadTexture("C:/Users/p4tri/FlappyBirdAssets/background.jpg", renderer);
        if (!backgroundTexture) {
            printf("Failed to load background texture! SDL_image Error: %s\n", IMG_GetError());
        }

        squirrelTexture = TextureManager::loadTexture("C:/Users/p4tri/FlappyBirdAssets/background.jpg", renderer);
        if (!squirrelTexture) {
            printf("Failed to load squirrel texture! SDL_image Error: %s\n", IMG_GetError());
        }
    }

    ~Gameplay() {
        if (backgroundTexture) {
            SDL_DestroyTexture(backgroundTexture);
        }
        if (squirrelTexture) {
            SDL_DestroyTexture(squirrelTexture);
        }
    }

    void run() {
        SDL_Event e;
        bool quit = false;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                    squirrel.jump();
                }
            }

            squirrel.update();

            SDL_RenderClear(renderer);
            renderObjects();
            SDL_RenderPresent(renderer);
        }
    }

private:
    SDL_Renderer* renderer;
    Squirrel squirrel;
    SDL_Texture* backgroundTexture;
    SDL_Texture* squirrelTexture;

    void renderObjects() {
        // Clear the screen with black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Clear color: black
        SDL_RenderClear(renderer);

        // Render the background
        if (backgroundTexture) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        }

        // Render the squirrel
        if (squirrelTexture) {
            SDL_Rect squirrelRect = { squirrel.x, squirrel.y, squirrel.width, squirrel.height };
            SDL_RenderCopy(renderer, squirrelTexture, nullptr, &squirrelRect);
        }

        // Present the rendered objects
        SDL_RenderPresent(renderer);
    }
};

#endif
