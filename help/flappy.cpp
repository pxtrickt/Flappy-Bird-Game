#include <iostream>
#include "Game.h"      

int main(int argc, char* args[]) {
    std::cout << "Initializing game..." << std::endl;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    // Load music
    Mix_Music* backgroundMusic = Mix_LoadMUS("C:/Users/p4tri/FlappyBirdAssets/music.mp3");

    // loop music
    if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
        std::cerr << "Failed to play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    Game game;
    if (!game.initialize()) {
        std::cerr << "Failed to initialize the game." << std::endl;
        return -1;
    }

    std::cout << "Game initialized successfully." << std::endl;
    game.run();

    // clean up
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();

    return 0;
}