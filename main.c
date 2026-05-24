#include "header.h"

int main() {
    initSDL();

    SDL_Surface *background;
    Button buttons[4];
    Mix_Music *bgMusic;
    Mix_Chunk *clickSound;

    loadAssets(&background, buttons, &bgMusic, &clickSound);

    bool running = true;
    GameState gameState = MENU;

    while (running) {
        handleEvents(&running, &gameState, buttons, clickSound);
        render(screen, background, buttons, gameState);
        SDL_Delay(16);
    }

    cleanup(background, buttons, bgMusic, clickSound);
    return 0;
}
