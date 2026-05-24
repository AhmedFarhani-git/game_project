#ifndef HEADER_H
#define HEADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

typedef enum {
    MENU
} GameState;

typedef struct {
    SDL_Rect rect;
    SDL_Surface *image;
    SDL_Surface *hoverImage;
    bool isHovered;
} Button;

extern SDL_Surface *screen; 

void initSDL();
void loadAssets(SDL_Surface **background, Button buttons[], Mix_Music **bgMusic, Mix_Chunk **clickSound);
void handleEvents(bool *running, GameState *gameState, Button buttons[], Mix_Chunk *clickSound);
void render(SDL_Surface *screen, SDL_Surface *background, Button buttons[], GameState gameState);
void cleanup(SDL_Surface *background, Button buttons[], Mix_Music *bgMusic, Mix_Chunk *clickSound);
bool isInside(int x, int y, SDL_Rect rect);

#endif
