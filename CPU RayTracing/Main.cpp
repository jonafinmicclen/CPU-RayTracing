#include <iostream>
#include "Utility.h"
#include "Perspective.h"
#include <SDL.h>
#include "Tests.h"

void drawPixel(SDL_Renderer* renderer, int x, int y, vec3 color) 
{
    SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

int main(int argc, char* args[]) 
{

    // Variables for easy control over animation
    const vec3 MODEL_POSITION = { 0,0,2 };
    const rotator ROTATION = { {1,1,0}, 0.01, MODEL_POSITION };

    const int SCREEN_WIDTH = 200;
    const int SCREEN_HEIGHT = 200;

    // Initialise a model for rendering
    standardModel stdModel = createCube(2);
    stdModel.translate(MODEL_POSITION);
    triangularModel triCube = stdModel.convertToTriModel();

    // Initalise a light source for rendering
    lightSource lSource;
    lSource.position = { 0,-2,0 };

    // Init perspective for rendering
    Perspective* perspective;
    perspective = new Perspective();
    perspective->generateInitRayArr();

    // Add our previously generated models
    perspective->scene_p.sceneFreeTriModels.push_back(triCube);
    perspective->scene_p.sceneFixedLightSources.push_back(lSource);

    // Create SDL context
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ray-Tracing CPU", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Draw logic
        perspective->clearScreen();
        perspective->calculateScreenArr();
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                drawPixel(renderer, x, y, perspective->ScreenArr[x][y]);
            }
        }

        // Rotate logic
        triCube.rotateInPlace(ROTATION);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete perspective;
    return 0;
}