#include <iostream>
#include "Utility.h"
#include "Perspective.h"

// Create a cube for testing
standardModel createCube(double sideLength, const vec3& position = { 0, 0, 0 }) {
    // Define the vertices of the cube
    vec3 v1 = { -sideLength / 2, -sideLength / 2, -sideLength / 2 };
    vec3 v2 = { sideLength / 2, -sideLength / 2, -sideLength / 2 };
    vec3 v3 = { sideLength / 2, sideLength / 2, -sideLength / 2 };
    vec3 v4 = { -sideLength / 2, sideLength / 2, -sideLength / 2 };
    vec3 v5 = { -sideLength / 2, -sideLength / 2, sideLength / 2 };
    vec3 v6 = { sideLength / 2, -sideLength / 2, sideLength / 2 };
    vec3 v7 = { sideLength / 2, sideLength / 2, sideLength / 2 };
    vec3 v8 = { -sideLength / 2, sideLength / 2, sideLength / 2 };

    // Translate vertices to the specified position
    v1 = v1 + position;
    v2 = v2 + position;
    v3 = v3 + position;
    v4 = v4 + position;
    v5 = v5 + position;
    v6 = v6 + position;
    v7 = v7 + position;
    v8 = v8 + position;

    // Define the surfaces of the cube using vertex indices
    std::vector<std::tuple<int, int, int>> surfaces = {
        {0, 1, 2}, {2, 3, 0},
        {4, 5, 6}, {6, 7, 4},
        {0, 4, 7}, {7, 3, 0},
        {1, 5, 6}, {6, 2, 1},
        {0, 1, 5}, {5, 4, 0},
        {2, 3, 7}, {7, 6, 2}
    };

    // Create the cube instance of standardModel
    return { {v1, v2, v3, v4, v5, v6, v7, v8}, surfaces };
}

int main() {

    standardModel cube = createCube(10);
    triangularModel triCube = cube.convertToTriModel();

    Perspective* p;
    p = new Perspective();

    p->generateInitRayArr();
    p->calculateScreenArr(triCube);

    delete p;

    return 0;
}