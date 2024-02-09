#pragma once

#include "Utility.h"

// Create a cube for testing
inline standardModel createCube(double sideLength) {
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
inline standardModel createDodecahedron(double sideLength) {
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