#include "header.h"
#include <stdio.h>
#include <stdlib.h>

SDL_Surface *screen = NULL;

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        exit(1);
    }
    
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    if (!screen) {
        printf("Video Mode Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption("Game Menu", NULL);

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("SDL_mixer Initialization Error: %s\n", Mix_GetError());
        exit(1);
    }
}

void loadAssets(SDL_Surface **background, Button buttons[], Mix_Music **bgMusic, Mix_Chunk **clickSound) {
    *background = IMG_Load("background.png");
    if (!*background) {
        printf("Failed to load background: %s\n", IMG_GetError());
        exit(1);
    }

    // Initialize all buttons
    for (int i = 0; i < 4; i++) {
        buttons[i].image = NULL;
        buttons[i].hoverImage = NULL;
        buttons[i].isHovered = false;
    }

    // Button image files [normal, hover]
    const char* buttonFiles[4][2] = {
        {"save_game_bt.png", "save_game_bt_2.png"},
        {"new_game_bt.png", "new_game_bt_2.png"},
        {"load_game_bt.png", "load_game_bt_2.png"},
        {"quit_bt1.png", "quit_bt2.png"}  // Exit button with hover
    };

    // Load button images
    for (int i = 0; i < 4; i++) {
        buttons[i].image = IMG_Load(buttonFiles[i][0]);
        if (!buttons[i].image) {
            printf("Failed to load button %d image: %s\n", i, IMG_GetError());
            exit(1);
        }

        buttons[i].hoverImage = IMG_Load(buttonFiles[i][1]);
        if (!buttons[i].hoverImage) {
            printf("Failed to load button %d hover image: %s\n", i, IMG_GetError());
            exit(1);
        }
    }

    // Set button positions (centered horizontally, spaced vertically)
    int startY = 400;
    int spacing = 150;
    for (int i = 0; i < 3; i++) {
        buttons[i].rect = (SDL_Rect){(SCREEN_WIDTH - buttons[i].image->w)/2, 
                                    startY + i*spacing, 
                                    buttons[i].image->w, 
                                    buttons[i].image->h};
    }
    // Position exit button at bottom right
    buttons[3].rect = (SDL_Rect){SCREEN_WIDTH - buttons[3].image->w - 50, 
                                 SCREEN_HEIGHT - buttons[3].image->h - 50, 
                                 buttons[3].image->w, 
                                 buttons[3].image->h};

    // Load audio
    *bgMusic = Mix_LoadMUS("1.mp3");
    *clickSound = Mix_LoadWAV("2.wav");
    if (!*bgMusic || !*clickSound) {
        printf("Failed to load audio: %s\n", Mix_GetError());
        exit(1);
    }

    Mix_PlayMusic(*bgMusic, -1);
}

bool isInside(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w && 
            y >= rect.y && y <= rect.y + rect.h);
}

void handleEvents(bool *running, GameState *gameState, Button buttons[], Mix_Chunk *clickSound) {
    SDL_Event event;
    int mouseX, mouseY;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *running = false;
                break;
                
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                for (int i = 0; i < 4; i++) {
                    buttons[i].isHovered = isInside(mouseX, mouseY, buttons[i].rect);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                mouseX = event.button.x;
                mouseY = event.button.y;
                for (int i = 0; i < 4; i++) {
                    if (isInside(mouseX, mouseY, buttons[i].rect)) {
                        Mix_PlayChannel(-1, clickSound, 0);
                        if (i == 3) { // Exit button
                            *running = false;
                            SDL_Delay(200);
                        }
                        // Add other button actions here if needed
                    }
                }
                break;
        }
    }
}

void render(SDL_Surface *screen, SDL_Surface *background, Button buttons[], GameState gameState) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    for (int i = 0; i < 4; i++) {
        SDL_Surface *img = buttons[i].isHovered ? buttons[i].hoverImage : buttons[i].image;
        if (img) {
            SDL_BlitSurface(img, NULL, screen, &buttons[i].rect);
        }
    }

    SDL_Flip(screen);
}

void cleanup(SDL_Surface *background, Button buttons[], Mix_Music *bgMusic, Mix_Chunk *clickSound) {
    SDL_FreeSurface(background);
    
    for (int i = 0; i < 4; i++) {
        if (buttons[i].image) SDL_FreeSurface(buttons[i].image);
        if (buttons[i].hoverImage) SDL_FreeSurface(buttons[i].hoverImage);
    }

    if (bgMusic) Mix_FreeMusic(bgMusic);
    if (clickSound) Mix_FreeChunk(clickSound);
    
    Mix_CloseAudio();
    SDL_Quit();
}
