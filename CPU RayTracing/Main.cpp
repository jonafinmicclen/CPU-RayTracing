#include <iostream>
#include "Utility.h"
#include "Perspective.h"
#include <SDL.h>

// Create a cube for testing
standardModel createCube(double sideLength) {
    // Define the vertices of the cube
    vec3 v1 = { -sideLength / 2, -sideLength / 2, -sideLength / 2 };
    vec3 v2 = { sideLength / 2, -sideLength / 2, -sideLength / 2 };
    vec3 v3 = { sideLength / 2, sideLength / 2, -sideLength / 2 };
    vec3 v4 = { -sideLength / 2, sideLength / 2, -sideLength / 2 };
    vec3 v5 = { -sideLength / 2, -sideLength / 2, sideLength / 2 };
    vec3 v6 = { sideLength / 2, -sideLength / 2, sideLength / 2 };
    vec3 v7 = { sideLength / 2, sideLength / 2, sideLength / 2 };
    vec3 v8 = { -sideLength / 2, sideLength / 2, sideLength / 2 };

    // Define the surfaces of the cube using vertex indices
    std::vector<std::tuple<int, int, int>> surfaces = {
        {0, 1, 2}, {0,3,2},
        {4, 5, 6}, {4,7,6},
        {0, 4, 7}, {7,0,3},
        {1, 5, 6}, {6, 2, 1},
        {0, 1, 5}, {5, 4, 0},
        {2, 3, 7}, {7, 6, 2}
    };
    std::vector<material> materials = {
        {1,0,0}, {1,0,0},
        {0,1,0}, {0,1,0},
        {0,0,1}, {0,0,1},
        {1,0,0}, {1,0,0},
        {0,1,0}, {0,1,0},
        {0,0,1}, {0,0,1}
    };

    // Create the cube instance of standardModel
    return { {v1, v2, v3, v4, v5, v6, v7, v8}, surfaces, materials };
}

static void rotateVec3(vec3& vec, double angleRadians, const vec3 axis) {

    double cosAngle = cos(angleRadians);
    double sinAngle = sin(angleRadians);

    double oneMinusCos = 1.0 - cosAngle;

    double axisX = axis.x;
    double axisY = axis.y;
    double axisZ = axis.z;

    double newX = (cosAngle + oneMinusCos * axisX * axisX) * vec.x +
        (oneMinusCos * axisX * axisY - sinAngle * axisZ) * vec.y +
        (oneMinusCos * axisX * axisZ + sinAngle * axisY) * vec.z;

    double newY = (oneMinusCos * axisY * axisX + sinAngle * axisZ) * vec.x +
        (cosAngle + oneMinusCos * axisY * axisY) * vec.y +
        (oneMinusCos * axisY * axisZ - sinAngle * axisX) * vec.z;

    double newZ = (oneMinusCos * axisZ * axisX - sinAngle * axisY) * vec.x +
        (oneMinusCos * axisZ * axisY + sinAngle * axisX) * vec.y +
        (cosAngle + oneMinusCos * axisZ * axisZ) * vec.z;

    vec.x = newX;
    vec.y = newY;
    vec.z = newZ;
}

static void translate(vec3& vec, const vec3& translation) {
    vec.x += translation.x;
    vec.y += translation.y;
    vec.z += translation.z;
}

static void rotateVec3AroundPoint(vec3& vec, double angleRadians, const vec3& axis, const vec3& point) {
    translate(vec, { -point.x, -point.y, -point.z });
    rotateVec3(vec, angleRadians, axis);
    translate(vec, { point.x, point.y, point.z });
}

const int SCREEN_WIDTH = 100;
const int SCREEN_HEIGHT = 100;

void drawPixel(SDL_Renderer* renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

int main(int argc, char* args[]) {

    // Initialise cube model for rendering
    standardModel cube = createCube(9);
    for (auto& v : cube.vertices) {
        v = v + vec3({0, -2, 20});
    }
    triangularModel triCube = cube.convertToTriModel();

    // Init perspective for rendering
    Perspective* p;
    p = new Perspective();
    p->generateInitRayArr();

    // Create SDL context
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Pixel Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
        p->clearScreen();
        p->calculateScreenArr(triCube);
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                drawPixel(renderer, x, y, p->ScreenArr[x][y].x * 255, p->ScreenArr[x][y].y * 255, p->ScreenArr[x][y].z * 255);
            }
        }

        // Rotate logic
        for (auto& v : cube.vertices) {
            rotateVec3AroundPoint(v, 0.01, { 0,1,0 }, { 0,-2,20 });
        }
        triCube = cube.convertToTriModel();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete p;

    return 0;
}