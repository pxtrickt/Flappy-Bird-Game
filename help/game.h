#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const int SQUIRREL_WIDTH = 55;
const int SQUIRREL_HEIGHT = 55;
const int ENEMY_HEIGHT = 40;
const int ENEMY_WIDTH = 40;
const float GRAVITY = 0.5f;
const float JUMP_POWER = -9.0f;
const int PIPE_WIDTH = 80;
const int PIPE_GAP = 200; 
const int PIPE_VELOCITY = 3;

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();

private:
    void cleanup();
    TTF_Font* font;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* squTexture;
    SDL_Texture* backgroundTexture;
    SDL_Texture* treeTexture;
    SDL_Texture* enemyTexture;
    Mix_Chunk* jumpSound = nullptr;
    int squX, squY;
    int enemyX, enemyY;
    float squVelocity;
    bool quit;
    bool squJump;
};

#endif 