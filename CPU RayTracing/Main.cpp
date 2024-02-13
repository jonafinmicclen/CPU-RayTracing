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
        {0.5,0.5,0}, {0.5,0.5,0},
        {0,0.5,0.5}, {0,0.5,0.5},
        {0.5,0,0.5}, {0.5,0,0.5}
    };

    // Create the cube instance of standardModel
    return { {v1, v2, v3, v4, v5, v6, v7, v8}, surfaces, materials };
}

// Dodecahedron for testing
standardModel createDodecahedron(double sideLength) {
    // Golden ratio
    double phi = (1 + sqrt(5)) / 2;

    // Define the vertices of the dodecahedron
    vec3 v1 = { -sideLength / 2, -sideLength / 2, -sideLength / (2 * phi) };
    vec3 v2 = { sideLength / 2, -sideLength / 2, -sideLength / (2 * phi) };
    vec3 v3 = { -sideLength / (2 * phi), 0, -phi * sideLength / 2 };
    vec3 v4 = { sideLength / (2 * phi), 0, -phi * sideLength / 2 };
    vec3 v5 = { 0, -phi * sideLength / 2, -sideLength / 2 };
    vec3 v6 = { 0, -phi * sideLength / 2, sideLength / 2 };
    vec3 v7 = { -sideLength / 2, sideLength / 2, sideLength / (2 * phi) };
    vec3 v8 = { sideLength / 2, sideLength / 2, sideLength / (2 * phi) };
    vec3 v9 = { -sideLength / (2 * phi), 0, phi * sideLength / 2 };
    vec3 v10 = { sideLength / (2 * phi), 0, phi * sideLength / 2 };
    vec3 v11 = { 0, phi * sideLength / 2, -sideLength / 2 };
    vec3 v12 = { 0, phi * sideLength / 2, sideLength / 2 };

    // Define the surfaces of the dodecahedron using vertex indices
    std::vector<std::tuple<int, int, int>> surfaces = {
        {1, 9, 5}, {1, 5, 2}, {2, 5, 12},
        {2, 12, 8}, {8, 12, 4}, {4, 12, 11},
        {4, 11, 7}, {7, 11, 3}, {3, 11, 10},
        {3, 10, 6}, {6, 10, 1}, {1, 10, 9},
        {9, 8, 5}, {5, 8, 12}, {12, 8, 4},
        {4, 7, 11}, {11, 7, 3}, {3, 6, 10},
        {10, 6, 1}, {9, 10, 8}, {8, 10, 4},
        {5, 12, 6}, {6, 12, 11}, {7, 3, 2}
    };

    std::vector<material> materials = {
        {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
        {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
        {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
        {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
        {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
        {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
        {1, 1, 0}, {1, 1, 0}, {1, 1, 0},
        {1, 1, 0}, {1, 1, 0}, {1, 1, 0}
    };

    // Create the dodecahedron instance of standardModel
    return { {v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12}, surfaces, materials };
}


// Pyramid for testing
standardModel createPyramid(double baseLength, double height) {
    // Define the vertices of the pyramid
    vec3 v1 = { 0, 0, 0 };
    vec3 v2 = { baseLength / 2, 0, -baseLength / 2 * sqrt(3) };
    vec3 v3 = { -baseLength / 2, 0, -baseLength / 2 * sqrt(3) };
    vec3 v4 = { 0, height, 0 };

    // Define the surfaces of the pyramid using vertex indices
    std::vector<std::tuple<int, int, int>> surfaces = {
        {0, 1, 2},
        {0, 1, 3},
        {1, 2, 3},
        {2, 0, 3}
    };

    std::vector<material> materials = {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {0.5,0.5,0}
    };

    // Create the pyramid instance of standardModel
    return { {v1, v2, v3, v4}, surfaces, materials };
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

void drawPixel(SDL_Renderer* renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

// Global variables for easy control over animation
const vec3 MODEL_POSITION = { 0,0,2 };
const vec3 ROTATION_AXIS = { 1,1,0 };
const double ROTATION_SPEED = 0.01;
const vec3 ROTATION_POINT = MODEL_POSITION;

const int SCREEN_WIDTH = 200;
const int SCREEN_HEIGHT = 200;

int main(int argc, char* args[]) {

    // Initialise a model for rendering
    standardModel stdModel = createCube(1);
    standardModel bigCube = createCube(200);
    //standardModel pyramid = 
    for (auto& v : bigCube.vertices) {
        v = v + vec3({ 0, 101, 0 });
    }

    //standardModel pyramid = 
    for (auto& v : stdModel.vertices) {
        v = v + vec3(MODEL_POSITION);
    }

    // Create models for perspective
    triangularModel triCube = stdModel.convertToTriModel();
    triangularModel tribigCube = bigCube.convertToTriModel(); 
    lightSource lSource;
    lSource.color = { 1,1,1 };
    lSource.position = { 0,-2,3 };
    lSource.radius = 0.8;

    // Create scene for perspective
    Scene newScene;
    newScene.light_sources.push_back(lSource);
    newScene.models.push_back(triCube);
    newScene.models.push_back(tribigCube);

    vec3 translation = { 0,2,0 };
    for (auto& tri : triCube.tris)
    {
        translate(tri.v1, translation);
        translate(tri.v2, translation);
        translate(tri.v3, translation);

    }

    //newScene.models.push_back(triCube);

    // Init perspective for rendering
    Perspective* p;
    p = new Perspective();
    p->scene = newScene;
    p->generateInitRayArr();

    // Create SDL context
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Ray-Tracing CPU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
        //p->clearScreen();
        p->resetPaths();
        p->testScreenArrFill();
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                drawPixel(renderer, x, y, p->ScreenArr[x][y].x * 255, p->ScreenArr[x][y].y * 255, p->ScreenArr[x][y].z * 255);
            }
        }

        // Rotate logic
        for (auto& triangle : p->scene.models[0].tris) {
            rotateVec3AroundPoint(triangle.v1, ROTATION_SPEED, ROTATION_AXIS, ROTATION_POINT);
            rotateVec3AroundPoint(triangle.v2, ROTATION_SPEED, ROTATION_AXIS, ROTATION_POINT);
            rotateVec3AroundPoint(triangle.v3, ROTATION_SPEED, ROTATION_AXIS, ROTATION_POINT);
            triangle.calculateNormal();
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete p;

    return 0;
}