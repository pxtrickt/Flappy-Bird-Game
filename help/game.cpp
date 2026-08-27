#include "game.h"
#include "menu.h"
#include "deathScreen.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>

//game constructer
Game::Game() : window(nullptr), renderer(nullptr), squTexture(nullptr),
backgroundTexture(nullptr), treeTexture(nullptr), font(nullptr),
squX(100), squY(SCREEN_HEIGHT / 2), squVelocity(0.0f), quit(false), squJump(false) {}

Game::~Game() {
    cleanup();
} 

void loadTexture(SDL_Renderer* renderer, SDL_Texture*& texture, const char* path) {
    texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        std::cerr << "Failed to load texture from " << path << ": " << IMG_GetError() << std::endl;
    }
}

bool Game::initialize() {
    // texture loading
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    backgroundTexture = IMG_LoadTexture(renderer, "C:/Users/p4tri/FlappyBirdAssets/backgroundMain.png");
    if (!backgroundTexture) {
        std::cerr << "Failed to load background texture: " << IMG_GetError() << std::endl;
        return false;
    }

    squTexture = IMG_LoadTexture(renderer, "C:/Users/p4tri/FlappyBirdAssets/squirrel.png");
    if (!squTexture) {
        std::cerr << "Failed to load squirrel texture: " << IMG_GetError() << std::endl;
        return false;
    }

    treeTexture = IMG_LoadTexture(renderer, "C:/Users/p4tri/FlappyBirdAssets/tree.png");
    if (!treeTexture) {
        std::cerr << "Failed to load pipe texture: " << IMG_GetError() << std::endl;
        return false;
    }

    enemyTexture = IMG_LoadTexture(renderer, "C:/Users/p4tri/FlappyBirdAssets/enemy.png");
    if (!enemyTexture) {
        std::cerr << "Failed to load enemy texture: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    jumpSound = Mix_LoadWAV("C:\\Users\\p4tri\\FlappyBirdAssets\\jump.wav"); 
    if (!jumpSound) {
        std::cerr << "Failed to load jump sound effect: " << Mix_GetError() << std::endl;
        return false;
    }


    return true;
}

void Game::run() {
    srand(static_cast<unsigned int>(time(0)));

    Menu menu(renderer, font);
    Death deathScreen(renderer);

    while (!quit) {
        // show Menu and wait for user input to start the game
        bool startGame = false;
        menu.show(quit, startGame);  

        if (quit) break;
        if (!startGame) continue;

        //variables
        squX = 100;
        squY = SCREEN_HEIGHT / 2;
        squVelocity = 0.0f;
        std::vector<SDL_Rect> pipes;
        int score = 0;
        int pipeTimer = 0;
     
        struct PipePair {
            SDL_Rect topPipe;
            SDL_Rect bottomPipe;
            bool scored = false;
        };

        std::vector<PipePair> pipePairs;
        SDL_Rect squRect = { squX, squY, SQUIRREL_WIDTH, SQUIRREL_HEIGHT };

        //enemy variables 

        float enemyAngle = 0.0f;
        int baseY = SCREEN_HEIGHT / 2;
        const int amplitude = 100;
        bool enemyActive = false;
        SDL_Rect enemyRect = { enemyX, enemyY, ENEMY_WIDTH, ENEMY_HEIGHT };

        //----------------- Game Loop 
        bool inGame = true;
        while (inGame && !quit) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                    squVelocity = JUMP_POWER;
                    if (jumpSound) {
                        Mix_PlayChannel(-1, jumpSound, 0);
                    }
                }
            }

            squVelocity += GRAVITY;
            squY += static_cast<int>(squVelocity);

            if (squY > SCREEN_HEIGHT - SQUIRREL_HEIGHT)
                squY = SCREEN_HEIGHT - SQUIRREL_HEIGHT;

            squRect.y = squY;

            pipeTimer++;
            if (pipeTimer > 90) {
                pipeTimer = 0;
                int minHeight = 50;
                int maxHeight = SCREEN_HEIGHT - PIPE_GAP - 50;
                int pipeHeight = rand() % (maxHeight - minHeight + 1) + minHeight;

                PipePair newPair;
                newPair.topPipe = { SCREEN_WIDTH, 0, PIPE_WIDTH, pipeHeight };
                newPair.bottomPipe = {
                    SCREEN_WIDTH,
                    pipeHeight + PIPE_GAP,
                    PIPE_WIDTH,
                    SCREEN_HEIGHT - (pipeHeight + PIPE_GAP)
                };
                pipePairs.push_back(newPair);
            }

            for (auto& pair : pipePairs) {
                pair.topPipe.x -= PIPE_VELOCITY;
                pair.bottomPipe.x -= PIPE_VELOCITY;

                if (!pair.scored && squX > pair.topPipe.x + PIPE_WIDTH) {
                    if (squY > pair.topPipe.h && squY + SQUIRREL_HEIGHT < pair.bottomPipe.y) {
                        pair.scored = true;
                        score++;
                        std::cout << "Score: " << score << std::endl;
                    }
                }
            }

            // ---------- removing off-screen pipes
            pipePairs.erase(std::remove_if(pipePairs.begin(), pipePairs.end(), [](const PipePair& pair) {
                return pair.topPipe.x + PIPE_WIDTH < 0;
                }), pipePairs.end());


            // ---------- enemy spawn & movement
            if (score >= 10 && !enemyActive) {
                enemyActive = true;
                enemyX = SCREEN_WIDTH;
                enemyAngle = 0.0f;
                baseY = rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT - 200) + 100;  // random baseY 
                enemyRect = { enemyX, baseY, ENEMY_WIDTH, ENEMY_HEIGHT };
            }

            if (enemyActive) {
                enemyX -= PIPE_VELOCITY + 1;
                enemyAngle += 0.1f;  // increase angle over time for smooth wave

                enemyY = static_cast<int>(baseY + amplitude * sinf(enemyAngle));

                // stay safe within screen bounds
                if (enemyY < 0) enemyY = 0;
                if (enemyY > SCREEN_HEIGHT - ENEMY_HEIGHT) enemyY = SCREEN_HEIGHT - ENEMY_HEIGHT;

                enemyRect.x = enemyX;
                enemyRect.y = enemyY;

                if (enemyX + ENEMY_WIDTH < 0) {
                    enemyX = SCREEN_WIDTH;
                    enemyAngle = 0.0f;
                    baseY = rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT - 200) + 100;
                }
            }


            // collision detection
            for (const auto& pair : pipePairs) {
                if (SDL_HasIntersection(&squRect, &pair.topPipe) ||
                    SDL_HasIntersection(&squRect, &pair.bottomPipe)) {
                    inGame = false;
                    break;
                }
            }

            if (enemyActive && SDL_HasIntersection(&squRect, &enemyRect)) {
                inGame = false;
            }

            // rendering
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

            for (const auto& pair : pipePairs) {
                SDL_RenderCopy(renderer, treeTexture, nullptr, &pair.topPipe);
                SDL_RenderCopy(renderer, treeTexture, nullptr, &pair.bottomPipe);
            }

            if (enemyActive) {
                SDL_RenderCopy(renderer, enemyTexture, nullptr, &enemyRect);
            }

            SDL_RenderCopy(renderer, squTexture, nullptr, &squRect);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);

          
        }
        // death screen displays
        if (!quit) {
            std::cout << "------------------------------------------------------------------" << std::endl;
            std::cout << "                   Your score was: " << score << std::endl;
            std::cout << "------------------------------------------------------------------" << std::endl;
            bool restartGame = false;
            deathScreen.show(quit, restartGame);
            if (!restartGame) quit = true;
        }
    }

}


void Game::cleanup() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
    if (squTexture) {
        SDL_DestroyTexture(squTexture);
        squTexture = nullptr;
    }
    if (treeTexture) {
        SDL_DestroyTexture(treeTexture);
        treeTexture = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    if (enemyTexture) {
        SDL_DestroyTexture(enemyTexture);
        enemyTexture = nullptr;
    }
    IMG_Quit();
    SDL_Quit();
}